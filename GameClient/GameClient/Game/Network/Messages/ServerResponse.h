#pragma once
#include "NetMessage.h"

class ServerResponse : public NetMessage
{
	using super = NetMessage;
public:
	ServerResponse();
	~ServerResponse() = default;

	void SetText(char const& text) { m_serverName = text; }
	std::string const& GetText() const { return m_serverName; }

	int serialize(ByteStream& stream) const override;
	int deserialize(ByteStream& stream) override;

private:
	std::string m_serverName;
};

