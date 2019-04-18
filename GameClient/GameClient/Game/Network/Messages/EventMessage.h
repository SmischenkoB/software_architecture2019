#pragma once

#include "NetMessage.h"

template <NetMessageType T>
class EventMessage : public NetMessage
{
	using super = NetMessage;
public:

	EventMessage() : super(T) {}
};
