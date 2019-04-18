#pragma once
class NetMessage;

struct ReceivedPair
{
	std::unique_ptr <NetMessage> message;
	NetAddr addr;
};

class MessageReciever
{
public:
	MessageReciever();
	~MessageReciever();

	void getMessages(std::vector<ReceivedPair>& buffer);

	void setSocket(NetSocket* const socket) { m_socket = socket; }
	void startReceiving();
	void stopReceiving();
	bool isReceiving() const { return m_receiverThread.joinable(); }

	void clearMessages();
	int getBytesRecieved()const { return m_bytesRecieved; }
	void resetBytesRecieved() { m_bytesRecieved = 0; }

private:
	void receiveMessages();
	std::unique_ptr <NetMessage> getMessageFromStream(ByteStream& stream) const;

	NetSocket* m_socket;
	std::vector <ReceivedPair> m_netMessages;
	std::thread m_receiverThread;
	std::mutex m_mutex;
	std::atomic_bool m_shouldReceive;

	int m_bytesRecieved = 0;
};

