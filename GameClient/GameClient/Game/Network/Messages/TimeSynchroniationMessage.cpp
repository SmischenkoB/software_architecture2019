#include "stdafx.h"
#include "TimeSynchroniationMessage.h"


TimeSynchroniationMessage::TimeSynchroniationMessage()
{
	m_messageType = NetMessageType::TIME_SYNCHRONIZATION;
}

int TimeSynchroniationMessage::serialize(ByteStream & stream) const
{
	int bytes = super::serialize(stream);

	bytes += stream.Serialize(time);
	return bytes;
}

int TimeSynchroniationMessage::deserialize(ByteStream & stream)
{
	int bytes = super::deserialize(stream);

	bytes += stream.Deserialize(time);
	return bytes;
}
