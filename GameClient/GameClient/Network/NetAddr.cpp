#include "stdafx.h"

NetAddr::NetAddr(NetAddr const & other)
{
	CopyFrom(&other);
}

//================================================================================
void NetAddr::ResetData ()
{
	memset(&m_stAddr, 0, sizeof(m_stAddr));
	m_stAddr.sin_family = AF_INET;
	m_stAddr.sin_addr.s_addr = 0;
	m_stAddr.sin_port = 0;
}

//================================================================================
void NetAddr::CopyFrom (const NetAddr* pNetAddr)
{
	if (!pNetAddr || pNetAddr == this)
		return;
	memcpy (&m_stAddr, &(pNetAddr->m_stAddr), sizeof (m_stAddr));
}

//================================================================================
int NetAddr::GetLocalAddr (sysAddr* pAddr)
{
	unsigned long nAddr = GetLocalAddr();
	nAddr = ntohl(nAddr);

	if (nAddr != 0)
		sprintf_s ((char*)pAddr, sizeof(sysAddr), "%d.%d.%d.%d", nAddr>>24, (nAddr>>16)&0xFF, (nAddr>>8)&0xFF, nAddr&0xFF);
	else
		strncpy_s ((char*)pAddr, sizeof(sysAddr), "127.0.0.1", sizeof(pAddr->szIPAddr));

	return 0;
}

//================================================================================
unsigned long NetAddr::GetLocalAddr()
{
	char szLocalAddrName[128];

	if (0 == gethostname(szLocalAddrName, 128))
	{
		PADDRINFOA addrInfo;
		getaddrinfo(szLocalAddrName, NULL, NULL, &addrInfo);
		while (addrInfo != NULL && addrInfo->ai_family != AF_INET)
			addrInfo = addrInfo->ai_next;

		if (addrInfo != NULL)
			return ((sockaddr_in*)addrInfo->ai_addr)->sin_addr.S_un.S_addr;
	}

	return 0;
}

//================================================================================
bool NetAddr::CreateByAddress (sysAddr *pAddr, unsigned short usPort)
{
	if (!pAddr) return false;
	ResetData ();
	inet_pton(AF_INET, (char*)pAddr, &m_stAddr.sin_addr);
	m_stAddr.sin_port = htons (usPort);
	return true;
}

//================================================================================
bool NetAddr::CreateByAddress (IN_ADDR *pInAddr, unsigned short usPort)
{
	ResetData ();
	m_stAddr.sin_addr.s_addr = pInAddr->s_addr;
	m_stAddr.sin_port = htons (usPort);
	return true;
}

//================================================================================
bool NetAddr::CreateBroadcastAddr (unsigned short usPort)
{
	ResetData ();
	m_stAddr.sin_addr.s_addr = INADDR_BROADCAST;
	m_stAddr.sin_port = htons (usPort);
	return true;
}

//================================================================================
bool NetAddr::GetIPAddress (sysAddr* pAddr) const
{
	if (!pAddr) return false;
	if (m_stAddr.sin_addr.s_addr != 0)
	{
		unsigned int addr =ntohl(m_stAddr.sin_addr.s_addr);
		sprintf_s ((char*)(pAddr->szIPAddr), sizeof(sysAddr), "%d.%d.%d.%d", (int)(addr>>24), (int)((addr>>16)&0xFF),
			(int)((addr>>8)&0xFF), (int)(addr&0xFF));
	}
	else
		strncpy_s ((char*)(pAddr->szIPAddr), sizeof(sysAddr), "127.0.0.1", sizeof(pAddr->szIPAddr));
	return true;
}

//================================================================================
void NetAddr::SetPort (unsigned short usPort)
{
	m_stAddr.sin_port = htons (usPort);
}

bool NetAddr::operator ==(NetAddr const& other) const
{
	return IsEqualTo(&other);
}

bool NetAddr::operator !=(NetAddr const& other) const
{
	return !IsEqualTo(&other);
}