#include "stdafx.h"
#include "ServerResponse.h"

ServerResponse::ServerResponse()
{
	char userName[256];
	Engine::GetUser(userName, 256);

	m_serverName = userName;

	m_messageType = NetMessageType::SERVER_RESPONSE;
}

int ServerResponse::serialize(ByteStream& stream) const
{
	int bytes = super::serialize(stream);
	bytes += stream.Serialize(m_serverName);
	return bytes;
}

int ServerResponse::deserialize(ByteStream& stream)
{
	int bytes = super::deserialize(stream);
	bytes += stream.Deserialize(m_serverName);
	return bytes;
}
