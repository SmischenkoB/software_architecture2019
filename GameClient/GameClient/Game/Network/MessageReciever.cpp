#include "stdafx.h"
#include "MessageReciever.h"

#include "Messages/NetMessage.h"

MessageReciever::MessageReciever()
	:m_socket(nullptr)
{}

MessageReciever::~MessageReciever()
{
	stopReceiving();
}

void MessageReciever::getMessages(std::vector<ReceivedPair>& buffer)
{
	std::lock_guard <std::mutex> lk(m_mutex);
	std::swap(m_netMessages, buffer);
}

void MessageReciever::startReceiving()
{
	m_shouldReceive.store(true);
	m_receiverThread = std::thread([this] {receiveMessages(); });
}

void MessageReciever::stopReceiving()
{
	if (isReceiving())
	{
		m_shouldReceive.store(false);
		m_receiverThread.join();
	}
}

void MessageReciever::clearMessages()
{
	m_netMessages.clear();
}

void MessageReciever::receiveMessages()
{
	while (m_shouldReceive.load())
	{
		// this is actually a byte stream
		ByteStream* messageDataByteStream;

		bool disconnected;
		NetAddr peerAddr;

		int bytes = m_socket->RecvFrom(&messageDataByteStream, peerAddr, disconnected);

		if (!disconnected && bytes > 0)
		{
			//ByteStream& messageDataByteStream = static_cast <ByteStream&>(*stream);

			m_bytesRecieved += messageDataByteStream->GetSize();

			auto message = getMessageFromStream(*messageDataByteStream);
			if (message)
			{
				std::lock_guard <std::mutex> lk(m_mutex);
				m_netMessages.push_back({ std::move(message), peerAddr });
			}
		}
		else
		{
			// without it this game eats 16-18% of NVIDIA GeForce GTX 1060 6GB
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

std::unique_ptr<NetMessage> MessageReciever::getMessageFromStream(ByteStream& stream) const
{
	int netMessageTypeIntRep;
	stream.Deserialize(netMessageTypeIntRep);
	NetMessageType receivedMessageType = static_cast <NetMessageType>(netMessageTypeIntRep);

	auto API = NetObjectAPI::GetInstance();

	if (!API->MessageIsRegistered(receivedMessageType))
	{
		return nullptr;
	}

	auto message = API->createMessageOfType(receivedMessageType);
	message->deserialize(stream);

	return message;
}
