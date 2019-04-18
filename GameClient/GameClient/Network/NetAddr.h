#pragma once
#pragma comment(lib, "ws2_32.lib")

// -------------------------------------------------------------------------------------
// Class that encapsulates the network address managing
// -------------------------------------------------------------------------------------

typedef struct {
	char szIPAddr[16];
} sysAddr;

class NetSocket;
class NetAddr
{
	friend class NetSocket;

public:
	NetAddr () { ResetData (); }
	NetAddr(NetAddr const& other);
	virtual ~NetAddr () {}

	virtual void ResetData ();
	void CopyFrom (const NetAddr *pNetAddr);

	static int GetLocalAddr (sysAddr *pAddr);
	static unsigned long GetLocalAddr();

	bool CreateByAddress (sysAddr *pAddr, unsigned short usPort);
	bool CreateByAddress (IN_ADDR *pInAddr, unsigned short usPort);
	bool CreateBroadcastAddr (unsigned short usPort);

	bool GetIPAddress (sysAddr* pAddr) const;
	inline IN_ADDR* GetInAddress () const { return (IN_ADDR*)&m_stAddr.sin_addr; }
	inline unsigned short GetPort () const { return ntohs(m_stAddr.sin_port); }
	void SetPort (unsigned short nPort);

	inline bool IsEqualTo (const NetAddr *pNetAddr) const { return pNetAddr ? (m_stAddr.sin_addr.s_addr == pNetAddr->m_stAddr.sin_addr.s_addr && m_stAddr.sin_port == pNetAddr->m_stAddr.sin_port) : false; }
	inline bool IsBroadcastAddress() const { return (m_stAddr.sin_addr.s_addr == INADDR_BROADCAST); }

	bool operator ==(NetAddr const& other) const;
	bool operator !=(NetAddr const& other) const;

protected:
	inline const sockaddr_in* GetSockAddr() const { return &m_stAddr; }
	sockaddr_in		m_stAddr;

private:
	void SetSockAddr(const sockaddr_in & addrStruct) { m_stAddr = addrStruct; }
	inline bool IsEqualToSockAddr (const sockaddr* pAddrStruct) { return pAddrStruct ? (m_stAddr.sin_addr.s_addr == ((sockaddr_in*)pAddrStruct)->sin_addr.s_addr && m_stAddr.sin_port == ((sockaddr_in*)pAddrStruct)->sin_port) : false; }
};
