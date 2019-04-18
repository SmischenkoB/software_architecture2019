#include "stdafx.h"

#include "Messages/NetMessage.h"
#include "Messages/ChatMessage.h"
#include "Messages/EventMessage.h"
#include "Messages/ServerResponse.h"
#include "Game/Network/Messages/TimeSynchroniationMessage.h"

NetObjectAPI* NetObjectAPI::m_instance = nullptr;

void NetObjectAPI::Init(bool const isHost)
{
	if (m_instance != nullptr)
	{
		return;
	}

	NetworkAPI::Init();

	m_instance = new NetObjectAPI(isHost);
}

void NetObjectAPI::Shutdown()
{
	delete m_instance;
	m_instance = nullptr;

	NetworkAPI::Close();
}

void NetObjectAPI::InitMessageFactory()
{
	RegisterMessage <EventMessage <NetMessageType::FIND_SERVER_REQUEST>>();
	RegisterMessage <EventMessage <NetMessageType::SERVER_SHUTDOWN>>();
	RegisterMessage <EventMessage <NetMessageType::CLIENT_JOIN_REQUEST>>();
	RegisterMessage <EventMessage <NetMessageType::SERVER_JOIN_RESPONSE>>();
	RegisterMessage <ServerResponse>();

	RegisterMessage <ChatMessage>();

	RegisterMessage <EventMessage <NetMessageType::GAME_IN_PROGRESS_STATE>>();
	RegisterMessage <EventMessage <NetMessageType::GAME_OVER>>();
	RegisterMessage <EventMessage <NetMessageType::PAUSE_STATE>>();


	RegisterMessage <TimeSynchroniationMessage>();
	// TODO : may add more events in the future
}

NetObjectAPI::NetObjectAPI(bool const isHost)
	: m_isHost(isHost)
{

	if (isHost)
	{
		sysAddr systemAddr;
		NetAddr::GetLocalAddr(&systemAddr);

		NetAddr localAddr;
		bool addrSuccess = localAddr.CreateByAddress(&systemAddr, m_defaultPort);
		assert(addrSuccess);

		bool const sockSuccess = m_socket.Create(NETAPI_SOCK_TYPE_UDP, &localAddr);
		assert(sockSuccess);
	}
	else
	{
		bool const sockSuccess = m_socket.Create(NETAPI_SOCK_TYPE_UDP);
		assert(sockSuccess);
	}
	InitMessageFactory();

	m_receiver.setSocket(&m_socket);
	m_receiver.startReceiving();
}

NetObjectAPI::~NetObjectAPI()
{
	m_receiver.clearMessages();
	clearClientAddrList();
	clearNetObjectList();
}

void NetObjectAPI::updatePerfomanceKeys(float dt)
{
	m_keysUpdateTimer += dt;

	if (m_keysUpdateTimer >= FRAME_TIME*60)
	{
		m_keysUpdateTimer = 0;
		m_performanceKeys = { m_receiver.getBytesRecieved(), m_bytesSent };
		m_receiver.resetBytesRecieved();
		m_bytesSent = 0;
	}
}

bool NetObjectAPI::MessageIsRegistered(NetMessageType const type) const
{
	return m_messageCreators.find(type) != m_messageCreators.cend();
}

void NetObjectAPI::Update(float dt)
{
	std::vector<ReceivedPair> messages;

	m_receiver.getMessages(messages);

	for (auto& pair : messages)
	{
		NetObjectDescriptor const receiverDescriptor = pair.message->getDescriptor();
		auto it = m_netObjects.find(receiverDescriptor);

		if (it == m_netObjects.cend())
		{
			continue;
		}

		it->second->ReceiveMessage(*pair.message, pair.addr);
	}

	for (auto& it : m_netObjects)
	{
		if (it.second)
		{
			it.second->Update();
		}
	}
	updatePerfomanceKeys(dt);
}

NetObject* NetObjectAPI::createMasterNetObject(NetObjectDescriptor const& descriptor)
{
	bool const isMaster = true;
	NetObject* netObject = new NetObject(isMaster, descriptor);

	registerNetObject(descriptor, netObject);
	return netObject;
}

NetObject* NetObjectAPI::createReplicaNetObject(NetObjectDescriptor const& descriptor)
{
	bool const isMaster = false;
	NetObject* netObject = new NetObject(isMaster, descriptor);

	registerNetObject(descriptor, netObject);
	return netObject;
}

NetObject* NetObjectAPI::createThirdPartyNetObject(NetObjectDescriptor const& descriptor)
{
	bool const isMaster = m_isHost;
	NetObject* netObject = new NetObject(isMaster, descriptor);

	registerNetObject(descriptor, netObject);
	return netObject;
}

NetAddr const& NetObjectAPI::getHostAddress() const
{
	return m_hostAddr;
}

NetAddr const& NetObjectAPI::getLocalAddress()
{
	return *m_socket.getLocalAddress();
}

void NetObjectAPI::sendNetMessage(NetMessage& message, NetAddr const& recipient)
{
	ByteStream stream;
	message.serialize(stream);

	//BaseStream* baseStream = &stream;
	auto result = m_socket.SendTo(&stream, recipient);

	if (result == SOCKET_ERROR)
	{
		assert(!"Message sending error");
	}

	m_bytesSent += stream.GetSize();
}

void NetObjectAPI::registerNetObject(NetObjectDescriptor const& descriptor, NetObject* object)
{
	bool const netObjectRegistered = m_netObjects.find(descriptor) != m_netObjects.cend();

	if (netObjectRegistered)
	{
		assert(!"NetObject is already registered!");
		return;
	}

	m_netObjects[descriptor] = object;
}

void NetObjectAPI::unregisterNetObject(NetObjectDescriptor const& descriptor)
{
	auto it = m_netObjects.find(descriptor);

	if (it == m_netObjects.cend())
	{
		assert(!"Deletion of not registered NetObject");
		return;
	}

	m_netObjects.erase(it);
}

void NetObjectAPI::setBroadcastMode(bool const value)
{
	bool const setSuccess = m_socket.SetOption(EInetSockOpt::NETAPI_SOCK_OPT_BROADCAST, value);
	assert(setSuccess);
}

void NetObjectAPI::clearClientAddrList()
{
	m_clients.clear();
}

void NetObjectAPI::registerClientAddr(NetAddr client)
{
	auto it = std::find(m_clients.begin(), m_clients.end(), client);

	if (it == m_clients.cend())
	{
		m_clients.push_back(client);
	}

	for (auto netObjects : m_netObjects)
	{
		netObjects.second->ReplicaAddedCallback(client);
	}
}

void NetObjectAPI::deleteClientAddr(NetAddr client)
{
	auto it = std::find(m_clients.begin(), m_clients.end(), client);
	if (it != m_clients.cend())
	{
		m_clients.erase(it);
	}

	for (auto netObjects : m_netObjects)
	{
		netObjects.second->ReplicaLeftCallback(client);
	}
}

void NetObjectAPI::clearNetObjectList()
{
	for (size_t i = 0; i < m_netObjects.size();)
	{
		std::unordered_map <NetObjectDescriptor, NetObject*>::iterator it = m_netObjects.begin();
		delete it->second;
	}

}

std::unique_ptr<NetMessage> NetObjectAPI::createMessageOfType(NetMessageType type)
{
	return m_messageCreators.at(type)();
}
