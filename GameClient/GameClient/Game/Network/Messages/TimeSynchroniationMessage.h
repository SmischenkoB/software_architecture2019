#pragma once
#include "NetMessage.h"

class TimeSynchroniationMessage : public NetMessage
{
	using super = NetMessage;
public:
	TimeSynchroniationMessage();
	~TimeSynchroniationMessage() = default;


	int serialize(ByteStream& stream) const override;
	int deserialize(ByteStream& stream) override;

	float time;

};

