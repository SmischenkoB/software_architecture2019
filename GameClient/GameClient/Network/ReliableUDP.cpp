#include "stdafx.h"

#define RUDP_DEFAULT_HEARTBEAT_INTERVAL 1
#define RUDP_DEFAULT_DISCONNECT_TIMEOUT RUDP_DEFAULT_HEARTBEAT_INTERVAL * 5
#define RUDP_DEFAULT_RETRANSMIT_TIMEOUT	0.5f


UDPChannel::UDPChannel(sockaddr_in & peerAddr)
{
	addr = peerAddr;
	Reset();

	handShakePacket.ResizeBuffer(sizeof(long long));

	long long llData = SOCKET_VERSION;
	handShakePacket << llData;

	hearbeatPacket.ResizeBuffer(sizeof(long));
}

UDPChannel::~UDPChannel()
{
	Reset();
}

void UDPChannel::Reset()
{
	nextID = 1; // valid IDs start from 1; 0 is no ID
	lastACKReceived = 0;
	ACKToSend = 0;
	lastSendTimestamp = -1;
	heartbeatInterval = RUDP_DEFAULT_HEARTBEAT_INTERVAL;
	lastRecvTimestamp = -1;
	disconnectTimeout = RUDP_DEFAULT_DISCONNECT_TIMEOUT;
	retransmitTimeout = RUDP_DEFAULT_RETRANSMIT_TIMEOUT;

	for (UDPByteStream * stream : processedRecvPackets)
		delete stream;
	processedRecvPackets.clear();
	for (UDPByteStream * stream : processedSendPackets)
		delete stream;
	processedRecvPackets.clear();

	state = State::HANDSHAKE;
}

void UDPChannel::AddRecv(char * data, int len)
{
	if (state == State::DISCONNECTED)
		return;

	UDPByteStream * savedStream = new UDPByteStream(data, len);

	int ackID;

	// reset the last received timestamp
	lastRecvTimestamp = clock();

	// read ACK ID & reliability flag
	*savedStream >> ackID;
	// find out if this packet is reliable or unreliable
	savedStream->reliable = ((ackID & MSB32) == MSB32);

	ackID = ackID & (~MSB32);
	if (lastACKReceived < ackID)
	{
		// just received a new ACK, we need to remove the packets in the SendData that are no longer needed
		lastACKReceived = ackID;
		CleanupSendData();
	}

	if (savedStream->reliable)
	{
		// this ia a reliable packet, so it also contain a packet ID; read packet ID & ordrdered flag
		*savedStream >> savedStream->id;
		// find out if this packet needs to be ordered or not (default is yes)
		savedStream->ordered = ((savedStream->id & MSB32) == MSB32);
		savedStream->id = savedStream->id & (~MSB32);

		if (savedStream->id == ACKToSend + 1)
		{
			// this is the next expected reliable packet; find what's the correct ACK to send and insert this in the beginning of the list
			ACKToSend++;
			for (auto it = processedRecvPackets.begin(); it != processedRecvPackets.end();)
				if ((*it)->id == ACKToSend + 1)
				{
					ACKToSend++;
					if ((*it)->processed)
					{
						delete *it;
						it = processedRecvPackets.erase(it);
					}
					else
						it++;
				}
				else
					it++;
			processedRecvPackets.push_front(savedStream);
			return;
		}

		// find the right place where to insert in the received packet list
		for (std::list<UDPByteStream*>::iterator iter = processedRecvPackets.begin(); iter != processedRecvPackets.end(); iter++)
		{
			if ((*iter)->id == savedStream->id)
			{
				// duplicate packet; discard it
				delete savedStream;
				return;
			}
			else if ((*iter)->id > savedStream->id)
			{
				// insert this packet before the current (iterator) packet
				processedRecvPackets.insert(iter, savedStream);
				return;
			}
			iter++;
		}
		// no packets with higher ID found, so insert it at the end of the list
		processedRecvPackets.push_back(savedStream);
	}
	else
		// add stream at the end of the list of received packet list
		processedRecvPackets.push_back(savedStream);
}

void UDPChannel::AddRecvHandshake()
{
	if (state == State::HANDSHAKE)
		state = State::LIVE;
	lastRecvTimestamp = clock();
}

void UDPChannel::AddSend(ByteStream * stream, bool reliable, bool ordered)
{
	if (state == State::DISCONNECTED)
		return;

	UDPByteStream * savedStream = new UDPByteStream(*stream, reliable, ordered, ACKToSend, reliable ? nextID++ : 0);
	processedSendPackets.push_back(savedStream);
}

void UDPChannel::AddSend(const char * data, int len, bool reliable, bool ordered)
{
	if (state == State::DISCONNECTED)
		return;

	UDPByteStream * savedStream = new UDPByteStream(data, len, reliable, ordered, ACKToSend, reliable ? nextID++ : 0);
	processedSendPackets.push_back(savedStream);
}

ByteStream * UDPChannel::GetProcessedRecv()
{
	if (state == State::DISCONNECTED)
		return nullptr;

	auto it = processedRecvPackets.begin();
	while (it != processedRecvPackets.end())
	{
		// if unreliable, just dispatch
		if (!(*it)->reliable)
		{
			ByteStream * stream = *it;
			processedRecvPackets.erase(it);
			return stream;
		}

		// if reliable, and already processed, check to see if we need to remove it from the list
		if ((*it)->processed)
		{
			if ((*it)->id <= ACKToSend)
			{
				delete *it;
				it = processedRecvPackets.erase(it);
			}
			continue;
		}
		// if reliable and not processed, check to see if is either unordered or in order and process it
		// meaning, skip packets that are over ACKToSend and ordered
		if (!(*it)->ordered || (*it)->id <= ACKToSend)
		{
			ByteStream * stream;
			if ((*it)->id <= ACKToSend)
			{
				stream = *it;
				it = processedRecvPackets.erase(it);
			}
			else
			{
				(*it)->processed = true;
				stream = new ByteStream();
				stream->Swap(**it);
			}
			return stream;
		}
		it++;
	}

	clock_t currentTime = clock();
	if (currentTime - lastRecvTimestamp < disconnectTimeout * CLOCKS_PER_SEC)
	{
		Reset();
		state = State::DISCONNECTED;
	}

	return nullptr;
}

void UDPChannel::StartProcessSend()
{
	sendIterator = processedSendPackets.begin();
}

ByteStream * UDPChannel::GetProcessedSend()
{
	if (state == State::DISCONNECTED)
		return nullptr;

	clock_t currentTime = clock();

	// first handle handsahake, if we are in this state
	if (state == State::HANDSHAKE)
	{
		if (currentTime - lastSendTimestamp < heartbeatInterval * CLOCKS_PER_SEC)
		{
			lastSendTimestamp = currentTime;
			return &handShakePacket;
		}
		return nullptr;
	}

	// we need to get through all packets in a frame
	while (sendIterator != processedSendPackets.end())
	{
		// if this packet was either not transmitted OR it's passed its retransmit timeout, send it
		if (!(*sendIterator)->processed || currentTime - (*sendIterator)->timestamp < retransmitTimeout * CLOCKS_PER_SEC)
		{
			lastSendTimestamp = currentTime;

			(*sendIterator)->processed = true;
			(*sendIterator)->UpdateHeader(ACKToSend);
			(*sendIterator)->timestamp = currentTime;
			sendIterator++;
			return (*sendIterator);
		}
		sendIterator++;
	}

	// if not packets were sent in the last heartbeat interval, send a hearbeat packet
	if (currentTime - lastSendTimestamp < heartbeatInterval * CLOCKS_PER_SEC)
	{
		lastSendTimestamp = currentTime;
		hearbeatPacket.Reset();
		int data = ACKToSend;
		hearbeatPacket << data;
		return &hearbeatPacket;
	}

	// we went through all packets and now is time to remove unreliable packets from the send list
	sendIterator = processedSendPackets.begin();
	while (sendIterator != processedSendPackets.end())
	{
		if ((*sendIterator)->processed && !(*sendIterator)->reliable)
		{
			delete *sendIterator;
			sendIterator = processedSendPackets.erase(sendIterator);
		}
		else
			sendIterator++;
	}

	return nullptr;
}

void UDPChannel::CleanupSendData()
{
	auto it = processedSendPackets.begin();
	while (it != processedSendPackets.end())
	{
		if ((*it)->id < lastACKReceived)
		{
			delete (*it);
			processedSendPackets.erase(it);
		}
	}
}
