#include "stdafx.h"
#include "ChatMessage.h"

ChatMessage::ChatMessage()
{
	m_messageType = NetMessageType::CHAT_MESSAGE;
}

int ChatMessage::serialize(ByteStream& stream) const
{
	int bytes = super::serialize(stream);
	bytes += stream.Serialize(m_text);
	return bytes;
}

int ChatMessage::deserialize(ByteStream& stream)
{
	int bytes = super::deserialize(stream);
	bytes += stream.Deserialize(m_text);
	return bytes;
}
