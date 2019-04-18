#pragma once

#include "NetMessage.h"

class ChatMessage : public NetMessage
{
public:
	using super = NetMessage;

	ChatMessage();
	~ChatMessage() = default;

	void SetText(std::string text) { m_text = std::move(text); }
	std::string const& GetText() const { return m_text; }

	int serialize(ByteStream& stream) const override;
	int deserialize(ByteStream& stream) override;

private:
	std::string m_text;
};
