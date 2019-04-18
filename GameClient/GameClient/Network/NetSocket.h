#pragma once

typedef enum
{
	NETAPI_SOCK_TYPE_NONE	= 0,
	NETAPI_SOCK_TYPE_TCP	= 1,
	NETAPI_SOCK_TYPE_UDP	= 2,
	NETAPI_SOCK_TYPE_RUDP	= 3
} EInetSockType;

typedef enum
{
	NETAPI_SOCK_OPT_BLOCKING	= 1,
	NETAPI_SOCK_OPT_TCPDELAY	= 2,
	NETAPI_SOCK_OPT_BROADCAST	= 3,
	NETAPI_SOCK_OPT_SENDBUFFER	= 4,
	NETAPI_SOCK_OPT_RECVBUFFER	= 5

} EInetSockOpt;

#define NETAPI_MAX_AUTOBIND_RETRIES			1000
#define NETAPI_DEFAULT_BUFFER_SIZE			0xFFFF

class NetSocket
{
public:
	NetSocket ();
	~NetSocket();

	void ResetData ();
	void CopyFrom (const NetSocket *pSocket);

	bool Create (EInetSockType eSocketType, NetAddr *pLocalAddr = NULL, bool bAutoBind = false);

	bool Listen (int iRequests);
	bool Accept (NetSocket *pSocket);
	bool Connect (const NetAddr *pRemoteAddr);

	int Send (const char *pData, int iSize);
	int SendTo (const char *pData, int iSize, const NetAddr & peerAddr, bool reliable = false, bool ordered = false);
	int SendTo (ByteStream * inStream, const NetAddr & peerAddr, bool reliable = false, bool ordered = false);

	int Recv (char *pData, int iMaxSize);
	int RecvFrom (char *pData, int iMaxSize, NetAddr & peerAddr, bool & disconnected);
	int RecvFrom (ByteStream ** outStream, NetAddr & peerAddr, bool & disconnected);

	// call this once (at the end of) a frame, for RUDP sockets to process background stuff
	void UpdateSend();

	bool Shutdown ();
	// Use Close () to terminate the entire connection; for RUDP, you can terminate specific connections, by passing the who NetAddr
	bool Close (NetAddr * who = nullptr);

	bool SetOption (EInetSockOpt eSocketOption, bool bValue);
	bool SetOption (EInetSockOpt eSocketOption, unsigned long lValue);

	int GetLastError () 
	{ 
		return WSAGetLastError(); 
	}
	bool IsLastErrorBlockingError () 
	{ 
		return WSAGetLastError() == (int)WSAEWOULDBLOCK;
	}

	inline NetAddr* GetPeerAddress ()  { return m_bConnected ? &m_PeerAddr : NULL; }
	inline NetAddr* getLocalAddress ()  { return (m_nSocketID != INVALID_SOCKET) ? &m_LocalAddr : NULL; }
	inline bool IsConnected () const { return m_bConnected; }
	inline bool IsValid () const { return INVALID_SOCKET != m_nSocketID; }

protected:
	EInetSockType			m_eSocketType;
	SOCKET					m_nSocketID;
	bool					m_bConnected;
	NetAddr					m_PeerAddr;
	NetAddr					m_LocalAddr;

	std::list<UDPChannel*>	m_RUDPChannels;

private:
	int	RecvFrom (ByteStream ** outStream, char *pData, int iMaxSize, NetAddr & peerAddr, bool & disconnected);

	char					m_DefRecvBuffer[NETAPI_DEFAULT_BUFFER_SIZE];
};
