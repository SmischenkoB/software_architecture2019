#pragma once

#define MSB32 0x80000000
#define SOCKET_VERSION 0x0F0FFA29
#define HANDSHAKE_DATA (long long)SOCKET_VERSION
#define DISCONNECT_DATA 0LL
#define BROADCAST_HEADER 1LL

class UDPByteStream : public ByteStream
{
public:
	// for received packets - swap content
	UDPByteStream(char * data, int len) : ByteStream()
	{
		Reset();
		Serialize(data, len);
	}

	// for packets to send - add RUDP header and copy content
	UDPByteStream(const char * data, int len, bool isReliable, bool isOrdered, int ack, int id) : ByteStream()
	{
		Reset();

		reliable = isReliable;
		ordered = isOrdered;

		writePos = sizeof(int) * (isReliable ? 2 : 1);
		capacity = writePos + len;
		charBuffer = new char[capacity];
		memcpy_s(GetBuffer() + writePos, len, data, len);

		AddHeader(ack, id);
	}

	UDPByteStream(ByteStream & stream, bool isReliable, bool isOrdered, int ack, int id) : ByteStream()
	{
		Reset();

		reliable = isReliable;
		ordered = isOrdered;

		Copy(stream);
		capacity = writePos + sizeof(int) * (isReliable ? 2 : 1);
		char* newBuffer = new char[capacity];
		memcpy_s(newBuffer + (capacity - writePos), writePos, charBuffer, writePos);
		charBuffer = newBuffer;

		AddHeader(ack, id);
	}

	void Reset()
	{
		processed = false;
		timestamp = -1;
		id = -1;
		reliable = false;
		ordered = false;
	}

	void AddHeader(int ack, int id)
	{
		writePos = 0;
		if (reliable)
			ack = ack | MSB32;
		*this << ack;
		if (reliable)
		{
			if (ordered)
				id = id | MSB32;
			*this << id;
		}
		writePos = capacity;
	}

	void UpdateHeader(int ack)
	{
		writePos = 0;
		if (reliable)
			ack = ack | MSB32;
		*this << ack;
		writePos = capacity;
	}

	bool processed;
	int timestamp;
	int id;
	bool reliable;
	bool ordered;
};

class NetSocket;
class NetAddr;
class UDPChannel
{
public:
	enum class State : int
	{
		HANDSHAKE,
		LIVE,
		DISCONNECTED
	};

	static bool IsHandshakePacket(char* data, int len) { return len == sizeof(long long) && *(long long*)data == HANDSHAKE_DATA;  }
	static bool IsDisconnectPacket(char* data, int len) { return len == sizeof(long long) && *(long long*)data == DISCONNECT_DATA; }
	static bool IsBroadcastPacket(char* data, int len) { return len > sizeof(long long) && *(long long*)data == BROADCAST_HEADER; }
	static long long GetHandshakePacket() { return HANDSHAKE_DATA; }
	static long long GetDisconnectPacket() { return DISCONNECT_DATA; }
	static long long GetBroadcastHeader() { return BROADCAST_HEADER; }

	UDPChannel(sockaddr_in & peerAddr);
	~UDPChannel();

	void Reset();

	bool HasAddress(sockaddr_in & peerAddr) { return peerAddr.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr && peerAddr.sin_port == addr.sin_port;  }

	// called with a new packet received from the saved peer address
	void AddRecv(char* data, int len);
	void AddRecvHandshake();
	// called with a packet that need to be prepared before sending it to the peer address
	void AddSend(ByteStream * stream, bool reliable = true, bool ordered = false);
	void AddSend(const char* data, int len, bool reliable = true, bool ordered = false);
	// Get next prepared pakcet to be send; continue to poll for packets until nullptr is returned (nothing to send)
	void StartProcessSend();
	ByteStream* GetProcessedSend();
	// Get next processed packet that was received; continue to poll for packets until nullptr is returned (nothing to process)
	ByteStream* GetProcessedRecv();

	sockaddr_in		addr;
	State			state;

private:
	// called when new packets are added, to remove acknowledged packets
	void CleanupSendData();

	std::list<UDPByteStream*> processedSendPackets;
	std::list<UDPByteStream*> processedRecvPackets;

	int nextID;
	int lastACKReceived;
	int ACKToSend;

	clock_t lastSendTimestamp;
	float heartbeatInterval;

	clock_t lastRecvTimestamp;
	float disconnectTimeout;

	float retransmitTimeout;

	ByteStream handShakePacket;
	ByteStream hearbeatPacket;

	std::list<UDPByteStream*>::iterator sendIterator;
};
