#pragma once
#include "stdafx.h"


enum class NetMessageType
{
	FIND_SERVER_REQUEST
	, SERVER_RESPONSE
	, SERVER_SHUTDOWN
	, CLIENT_JOIN_REQUEST
	, SERVER_JOIN_RESPONSE
	, CHAT_MESSAGE
	, PAUSE_STATE
	, GAME_OVER
	, GAME_IN_PROGRESS_STATE
	, TIME_SYNCHRONIZATION
};


class NetMessage
{
public:
	NetMessage() = default;
	~NetMessage() = default;

	NetMessageType getNetMessageType() const { return m_messageType; }
	void setNetMessageType(NetMessageType newType) { m_messageType = newType; }

	NetObjectDescriptor getDescriptor() { return m_descriptor; }
	void setDescriptor(NetObjectDescriptor newDescriptor) { m_descriptor = newDescriptor; }

	virtual	int serialize(ByteStream& stream) const;
	virtual	int deserialize(ByteStream& stream);	

protected:
	NetMessage(NetMessageType type);

	NetMessageType m_messageType;

	NetObjectDescriptor m_descriptor;
};