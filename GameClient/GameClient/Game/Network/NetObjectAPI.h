#pragma once

#include "NetObject.h"
#include "MessageReciever.h"

class NetMessage;

enum class NetMessageType;

using NetMessageCreator = std::function <std::unique_ptr <NetMessage>()>;

struct PerformanceKeys
{
	int bytesRecieved;
	int bytesSent;
};

class NetObjectAPI
{
public:
	static void Init(bool const isHost = false);
	static void Shutdown();

	static NetObjectAPI* GetInstance() { return m_instance; }
	void InitMessageFactory();

	template <class T>
	void RegisterMessage();
	bool MessageIsRegistered(NetMessageType type) const;

	bool IsHost() const { return m_isHost; }
	void SetHost(bool const host) { m_isHost = host; }
	void Update(float dt); //Process messages, update net objects, etc.

	NetObject* createMasterNetObject(NetObjectDescriptor const& descriptor);
	NetObject* createReplicaNetObject(NetObjectDescriptor const& descriptor);

	// just a shortcut, creates master on host and replica on client, you need only this one until P2P
	NetObject* createThirdPartyNetObject(NetObjectDescriptor const& descriptor);

	NetAddr const& getHostAddress() const;
	void setHostAddress(NetAddr addr) { m_hostAddr = addr; }

	NetAddr const& getLocalAddress();

	unsigned short getDefaultPort() const { return m_defaultPort; }

	//This is called only by NetObjects
	void sendNetMessage(NetMessage& message, NetAddr const& recipient);

	void registerNetObject(NetObjectDescriptor const& descriptor, NetObject* object);
	void unregisterNetObject(NetObjectDescriptor const& descriptor);

	void setBroadcastMode(bool const value);

	std::vector<NetAddr> const& getClients() { return m_clients; }
	void clearClientAddrList();

	void registerClientAddr(NetAddr client);
	void deleteClientAddr(NetAddr client);

	void clearNetObjectList();
	std::unique_ptr <NetMessage> createMessageOfType(NetMessageType type);

	PerformanceKeys getPerformanceKeys()const { return m_performanceKeys; }

private:
	NetObjectAPI(bool const isHost);
	NetObjectAPI(NetObjectAPI const& other) = delete;
	~NetObjectAPI();

	void updatePerfomanceKeys(float dt);
private:

	std::unordered_map <NetObjectDescriptor, NetObject*> m_netObjects;
	std::unordered_map <NetMessageType, NetMessageCreator> m_messageCreators;

	std::vector<NetAddr> m_clients;

	NetSocket m_socket;
	NetAddr m_hostAddr;

	bool m_isHost;
	unsigned short const m_defaultPort = 1422;

	static NetObjectAPI* m_instance;
	MessageReciever m_receiver;

	PerformanceKeys m_performanceKeys;
	int m_bytesSent = 0;
	float m_keysUpdateTimer = 0;

};

template <class T>
inline void NetObjectAPI::RegisterMessage()
{
	static_assert(std::is_default_constructible_v <T>, "Message should have a default constructor!");
	T message;
	auto messageType = message.getNetMessageType();
	if (MessageIsRegistered(messageType))
	{
		return;
	}
	m_messageCreators[messageType] = [] { return std::make_unique <T>(); };
}
