#include "stdafx.h"
#include "NetMessage.h"

int NetMessage::serialize(ByteStream& stream) const
{
	int bytes = stream.Serialize(static_cast <int>(m_messageType));

	bytes += stream.Serialize(m_descriptor.m_instanceID);
	bytes += stream.Serialize(m_descriptor.objectType);

	return bytes;
}

int NetMessage::deserialize(ByteStream& stream)
{
	int bytes = stream.Deserialize(m_descriptor.m_instanceID);
	bytes += stream.Deserialize(m_descriptor.objectType);
	
	return bytes;
}

NetMessage::NetMessage(NetMessageType const type)
	: m_messageType(type)
{}
