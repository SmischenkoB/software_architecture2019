#include "stdafx.h"
#include "NetSocket.h"

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (~0)
#endif

//================================================================================
NetSocket::NetSocket()
{
	m_eSocketType = NETAPI_SOCK_TYPE_NONE;
	m_nSocketID = INVALID_SOCKET;
	m_bConnected = false;
}

//================================================================================
NetSocket::~NetSocket()
{
	Close();
}

//================================================================================
void NetSocket::ResetData()
{
	m_eSocketType = NETAPI_SOCK_TYPE_NONE;
	m_bConnected = false;
	m_nSocketID = INVALID_SOCKET;
	m_PeerAddr.ResetData();
}

//================================================================================
void NetSocket::CopyFrom(const NetSocket *pSocket)
{
	if (!pSocket || pSocket == this)
		return;
	m_eSocketType = pSocket->m_eSocketType;
	m_nSocketID = pSocket->m_nSocketID;
	m_bConnected = pSocket->m_bConnected;
	m_PeerAddr.CopyFrom(&pSocket->m_PeerAddr);
}

//================================================================================
bool NetSocket::Create(EInetSockType eSocketType, NetAddr *pLocalAddr/* = NULL*/, bool bAutoBind/* = false*/)
{
	if (IsValid())
	{
		return false;
	}

	m_eSocketType = eSocketType;
	m_nSocketID = INVALID_SOCKET;
	m_bConnected = false;

	if (eSocketType == NETAPI_SOCK_TYPE_TCP)
		m_nSocketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (eSocketType == NETAPI_SOCK_TYPE_UDP || eSocketType == NETAPI_SOCK_TYPE_RUDP)
		m_nSocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!IsValid())
	{
		return false;
	}

	if (pLocalAddr != NULL || eSocketType == NETAPI_SOCK_TYPE_UDP || eSocketType == NETAPI_SOCK_TYPE_RUDP)
	{
		// Socket bind necessary
		bool bBindError = false;
		int iBindResult;

		sockaddr_in localAddr;
		localAddr.sin_family = AF_INET;

		if (pLocalAddr == NULL)
		{
			localAddr.sin_addr.s_addr = INADDR_ANY;
			localAddr.sin_port = 0;
			iBindResult = bind(m_nSocketID, (sockaddr*)&localAddr, sizeof(localAddr));

			bBindError = (iBindResult == SOCKET_ERROR);
		}
		else
		{
			short usPort = pLocalAddr->GetPort();
			localAddr.sin_addr.s_addr = pLocalAddr->GetSockAddr()->sin_addr.s_addr;
			localAddr.sin_port = htons(usPort);

			iBindResult = bind(m_nSocketID, (sockaddr*)&localAddr, sizeof(localAddr));

			if (bAutoBind)
			{
				if (iBindResult == SOCKET_ERROR && WSAEADDRINUSE == GetLastError())
					bBindError = true;

				int iBindRetries = 0;
				while (bBindError && iBindRetries < NETAPI_MAX_AUTOBIND_RETRIES)
				{
					bBindError = false;
					iBindRetries++;
					usPort++;
					if (usPort == 0) usPort = 1000;
					localAddr.sin_port = htons(usPort);

					iBindResult = bind(m_nSocketID, (sockaddr*)&localAddr, sizeof(localAddr));

					if (iBindResult == SOCKET_ERROR && WSAEADDRINUSE == GetLastError())
						bBindError = true;
				}
			}
		}

		if (bBindError)
		{
			Close();
			return false;
		}
		else
		{
			int len = sizeof(localAddr);
			getsockname(m_nSocketID, (sockaddr*)&localAddr, &len);
			m_LocalAddr.CreateByAddress(&localAddr.sin_addr, ntohs(localAddr.sin_port));
		}
	}

	SetOption(NETAPI_SOCK_OPT_BLOCKING, false);
	if (eSocketType == NETAPI_SOCK_TYPE_TCP)
		SetOption(NETAPI_SOCK_OPT_TCPDELAY, false);

	return true;
}

//================================================================================
bool NetSocket::Listen(int iRequests)
{
	return (listen(m_nSocketID, iRequests) == SOCKET_ERROR) ? false : true;
}

//================================================================================
bool NetSocket::Accept(NetSocket *pSocket)
{
	if (pSocket == NULL) return false;

	sockaddr_in peerAddr;
	int iLen;
	iLen = sizeof(peerAddr);

	pSocket->m_nSocketID = accept(m_nSocketID, (sockaddr*)&peerAddr, &iLen);
	if (pSocket->m_nSocketID != SOCKET_ERROR)
	{
		pSocket->m_eSocketType = NETAPI_SOCK_TYPE_TCP;
		pSocket->m_PeerAddr.SetSockAddr(peerAddr);
		pSocket->m_bConnected = true;
		return true;
	}
	else
	{
		//TRACE0("Error in accept call (socket)\n");
		return false;
	}
}

//================================================================================
bool NetSocket::Connect(const NetAddr *pRemoteAddr)
{
	m_PeerAddr.CopyFrom(pRemoteAddr);

	if (m_eSocketType == NETAPI_SOCK_TYPE_TCP)
	{
		if (IsConnected())
			return false;

		if (0 != connect(m_nSocketID, (sockaddr*)m_PeerAddr.GetSockAddr(), sizeof(sockaddr)))
		{
			if (IsLastErrorBlockingError())
			{
				m_bConnected = true;
				return true;
			}
			else
			{

				//DWORD err = WSAGetLastError();
				m_bConnected = false;
				return false;
			}
		}
	}

	return false;
}

//================================================================================
int NetSocket::Send(const char *pData, int iSize)
{
	int retval = -1;

	if (m_eSocketType == NETAPI_SOCK_TYPE_TCP && IsValid() && IsConnected())
	{
		int cnt = 0;
		retval = 0;
		while ((iSize > 0) && (retval != SOCKET_ERROR))
		{
			retval = send(m_nSocketID, pData + cnt, iSize, 0);
			cnt += retval;
			iSize -= retval;
		}
	}
	return retval;

}

//================================================================================
int NetSocket::Recv(char *pData, int iMaxSize)
{
	int retval = -1;

	if (m_eSocketType == NETAPI_SOCK_TYPE_TCP && IsValid() && IsConnected())
		retval = recv(m_nSocketID, pData, iMaxSize, 0);

	return retval;
}

//================================================================================
int NetSocket::SendTo(const char *pData, int iSize, const NetAddr & peerAddr, bool reliable, bool ordered)
{
	if (!IsValid() || m_eSocketType == NETAPI_SOCK_TYPE_TCP)
		return -1;

	int retval = -1;
	if (m_eSocketType == NETAPI_SOCK_TYPE_UDP)
	{
		retval = sendto(m_nSocketID, pData, iSize, 0, (sockaddr*)peerAddr.GetSockAddr(), sizeof(sockaddr));
	}
	else if (m_eSocketType == NETAPI_SOCK_TYPE_RUDP)
	{
		if (peerAddr.IsBroadcastAddress())
		{
			char data[MAXSHORT];
			*(long long*)data = UDPChannel::GetBroadcastHeader();
			memcpy_s(data + sizeof(long long), MAXSHORT - sizeof(long long), pData, iSize);
			retval = sendto(m_nSocketID, data, iSize + sizeof(long long), 0, (sockaddr*)peerAddr.GetSockAddr(), sizeof(sockaddr));
			return retval;
		}

		sockaddr_in addr = *peerAddr.GetSockAddr();
		UDPChannel * foundChannel = nullptr;
		for (UDPChannel * channel : m_RUDPChannels)
			if (channel->HasAddress(addr))
			{
				foundChannel = channel;
				break;
			}
		// if there is none, create one
		if (!foundChannel)
		{
			foundChannel = new UDPChannel(addr); // new channels are in handshake mode
			m_RUDPChannels.push_back(foundChannel);
		}
		foundChannel->AddSend(pData, iSize, reliable, ordered);
	}
	return retval;
}

//================================================================================
int NetSocket::SendTo(ByteStream * inStream, const NetAddr & peerAddr, bool reliable, bool ordered)
{
	return SendTo(inStream->GetBuffer(), inStream->GetSize(), peerAddr, reliable, ordered);
}

//================================================================================
int NetSocket::RecvFrom(char * pData, int iMaxSize, NetAddr & peerAddr, bool & disconnected)
{
	if (!pData || iMaxSize <= 0)
		return -1;
	return RecvFrom(NULL, pData, iMaxSize, peerAddr, disconnected);
}

//================================================================================
int NetSocket::RecvFrom(ByteStream ** outStream, NetAddr & peerAddr, bool & disconnected)
{
	if (outStream == nullptr)
		return -1;
	return RecvFrom(outStream, NULL, 0, peerAddr, disconnected);
}

//================================================================================
int	NetSocket::RecvFrom(ByteStream ** outStream, char *pData, int iMaxSize, NetAddr & peerAddr, bool & disconnected)
{
	if (!IsValid() || m_eSocketType == NETAPI_SOCK_TYPE_TCP)
		return -1;

	disconnected = false;

	sockaddr_in addr;
	int iLen = sizeof(sockaddr);
	int retval = -1;

	retval = recvfrom(m_nSocketID, m_DefRecvBuffer, NETAPI_DEFAULT_BUFFER_SIZE, 0, (struct sockaddr*)&addr, &iLen);

	if (m_eSocketType == NETAPI_SOCK_TYPE_RUDP)
	{
		if (retval > 0)
		{
			// check to see if this packet is a handshake or a disconnect packet
			bool bHandshake = UDPChannel::IsHandshakePacket(pData, iLen);
			bool bDisconnect = UDPChannel::IsDisconnectPacket(pData, iLen);
			bool bBroadcast = UDPChannel::IsBroadcastPacket(pData, iLen);

			// if this is a broadcast packet, just dispatch it imediatelly
			if (bBroadcast)
			{
				peerAddr.SetSockAddr(addr);
				retval = iLen - sizeof(long long);
				if (pData)
				{
					if (iMaxSize >= retval)
					{
						memcpy_s(pData, iMaxSize, m_DefRecvBuffer + sizeof(long long), retval);
					}
				}
				else
				{
					ByteStream* stream = new ByteStream(retval);
					stream->Copy(m_DefRecvBuffer + sizeof(long long), retval);
				}
				return retval;
			}

			// search for the channel with this addr
			UDPChannel * foundChannel = nullptr;
			auto it = m_RUDPChannels.begin();
			while (it != m_RUDPChannels.end())
			{
				if ((*it)->HasAddress(addr))
				{
					foundChannel = *it;
					break;
				}
				it++;
			}

			// if there is none, create one
			if (!foundChannel)
			{
				if (bHandshake)
				{
					// this was a handshake packet and we need to add this channel to the list
					foundChannel = new UDPChannel(addr); // new channels are in handshake mode
					m_RUDPChannels.push_back(foundChannel);
					long long lHandshake = UDPChannel::GetHandshakePacket();
					sendto(m_nSocketID, (char*)&lHandshake, sizeof(lHandshake), 0, (sockaddr*)&addr, sizeof(sockaddr));
					foundChannel->AddRecvHandshake(); // handshake done; channel is now live
				}
				// otherwise just ignore and drop this packet
			}
			else {
				if (foundChannel->state == UDPChannel::State::HANDSHAKE)
				{
					if (bHandshake)
						foundChannel->AddRecvHandshake(); // handshake done; channel is now live
					// otherwise just ignore and drop this packet
				}
				else
				{
					if (bHandshake)
					{
						// this is a handshake packet, so the connection was remade; this is no longer the same connection as before
						// we need to inform the client the old peer has disconnected
						peerAddr.SetSockAddr(addr);
						disconnected = true; // announce that this channel has disconnected
						foundChannel->Reset(); // channel is back in handshake mode
						long long lHandshake = UDPChannel::GetHandshakePacket();
						sendto(m_nSocketID, (char*)&lHandshake, sizeof(lHandshake), 0, (sockaddr*)&addr, sizeof(sockaddr));
						foundChannel->AddRecvHandshake(); // handshake done; channel is live again
						return sizeof(int);
					}
					else if (bDisconnect)
					{
						// this is a disconnect packet, we need to invalidate this connection
						peerAddr.SetSockAddr(addr);
						disconnected = true; // announce that this channel has disconnected
						m_RUDPChannels.erase(it);
						delete foundChannel;
						return sizeof(int);
					}
					else
						foundChannel->AddRecv(m_DefRecvBuffer, iLen);
				}
			}
		}

		// iterate through the channels and return the first processed message
		auto it = m_RUDPChannels.begin();
		while (it != m_RUDPChannels.end())
		{
			ByteStream* stream = (*it)->GetProcessedRecv();
			if (!stream && (*it)->state == UDPChannel::State::DISCONNECTED)
			{
				// announce disconnected state and then drop the channel
				peerAddr.SetSockAddr((*it)->addr);
				disconnected = true;
				delete *it;
				m_RUDPChannels.erase(it);
				return sizeof(int);
			}
			if (stream)
			{
				int len = stream->GetSize();
				peerAddr.SetSockAddr((*it)->addr);
				if (pData)
				{
					if (iMaxSize >= stream->GetSize())
					{
						memcpy_s(pData, iMaxSize, stream->GetBuffer(), len);
						delete stream;
					}
				}
				else
				{
					*outStream = stream;
				}
				return len;
			}
			// if nothing to process, return 0
			return 0;
		}
	}
	else if (m_eSocketType == NETAPI_SOCK_TYPE_UDP)
	{
		if (retval > 0)
		{
			peerAddr.SetSockAddr(addr);
			if (pData)
			{
				if (iMaxSize >= retval)
				{
					memcpy_s(pData, iMaxSize, m_DefRecvBuffer, retval);
				}
			}
			else
			{
				*outStream = new ByteStream(retval);
				(*outStream)->Serialize(m_DefRecvBuffer, retval);
			}
		}
	}

	return retval;
}

//================================================================================
void NetSocket::UpdateSend()
{
	// function valid only for RUDP sockets
	if (!IsValid() || m_eSocketType != NETAPI_SOCK_TYPE_RUDP)
		return;

	UDPChannel * foundChannel = nullptr;
	for (UDPChannel * channel : m_RUDPChannels)
	{
		ByteStream * stream;
		channel->StartProcessSend();
		while ((stream = channel->GetProcessedSend()) != nullptr)
			int retval = sendto(m_nSocketID, stream->GetBuffer(), stream->GetSize(), 0, (sockaddr*)&channel->addr, sizeof(sockaddr));
	}
}

//================================================================================
bool NetSocket::Shutdown()
{
	//	if (IsValid())
	//		shutdown (m_nSocketID, SD_SEND);
	return true;
}

//================================================================================
bool NetSocket::Close(NetAddr * who)
{
	if (IsValid())
	{
		if (m_eSocketType == NETAPI_SOCK_TYPE_RUDP)
		{
			long long data = 0;

			sockaddr_in * sockAddr = who ? (sockaddr_in *)who->GetSockAddr() : nullptr;

			// be sure to send closure packet to the specific channel or all the channels of the RUDP
			auto it = m_RUDPChannels.begin();
			while (it != m_RUDPChannels.end())
			{
				if (!sockAddr || (*it)->HasAddress(*sockAddr))
					sendto(m_nSocketID, (char*)&data, sizeof(data), 0, (sockaddr*)&(*it)->addr, sizeof(sockaddr));
				it++;
			}
			if (!who)
				closesocket(m_nSocketID);
		}
		else
			closesocket(m_nSocketID);
	}
	ResetData();
	return true;
}

//================================================================================
bool NetSocket::SetOption(EInetSockOpt eSocketOption, bool bValue)
{
	if (m_nSocketID == SOCKET_ERROR)
		return false;

	unsigned long value;

	switch (eSocketOption)
	{
	case NETAPI_SOCK_OPT_BLOCKING:
		value = bValue ? 0 : 1;
		return (ioctlsocket(m_nSocketID, FIONBIO, &value) == SOCKET_ERROR) ? false : true;

	case NETAPI_SOCK_OPT_TCPDELAY:
		value = bValue ? 0 : 1;
		return (setsockopt(m_nSocketID, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(value)) == SOCKET_ERROR) ? false : true;

	case NETAPI_SOCK_OPT_BROADCAST:
		value = bValue ? 1 : 0;
		return (setsockopt(m_nSocketID, SOL_SOCKET, SO_BROADCAST, (char*)&value, sizeof(value)) == SOCKET_ERROR) ? false : true;

	case NETAPI_SOCK_OPT_SENDBUFFER:


	default:
		return false;
	}
}

//================================================================================
bool NetSocket::SetOption(EInetSockOpt eSocketOption, unsigned long lValue)
{
	if (m_nSocketID == SOCKET_ERROR)
		return false;

	switch (eSocketOption)
	{
	case NETAPI_SOCK_OPT_SENDBUFFER:
		return (setsockopt(m_nSocketID, SOL_SOCKET, SO_SNDBUF, (const char*)&lValue, 4) == SOCKET_ERROR);

	case NETAPI_SOCK_OPT_RECVBUFFER:
		return (setsockopt(m_nSocketID, SOL_SOCKET, SO_RCVBUF, (const char*)&lValue, 4) == SOCKET_ERROR);

	default:
		return false;
	}
}
