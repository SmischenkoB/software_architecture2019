#include "stdafx.h"
#include "NetObjectAPI.h"
#include "Messages/NetMessage.h"

NetObject::NetObject(bool const isMaster, NetObjectDescriptor const& descriptor)
	: m_descriptor(descriptor),
	m_isMaster(isMaster)
{
}

NetObject::~NetObject()
{
	NetObjectAPI::GetInstance()->unregisterNetObject(m_descriptor);
}

void NetObject::SendMasterBroadcast(NetMessage& message) const
{
	auto API = NetObjectAPI::GetInstance();
	message.setDescriptor(m_descriptor);

	for (auto const& addr : API->getClients())
	{
		API->sendNetMessage(message, addr);
	}
}

void NetObject::SendUnicastMessage(NetMessage& message, NetAddr const& addr) const
{
	auto API = NetObjectAPI::GetInstance();

	message.setDescriptor(m_descriptor);
	API->sendNetMessage(message, addr);
}

void NetObject::LANBroadcast(NetMessage& message) const
{
	auto API = NetObjectAPI::GetInstance();

	API->setBroadcastMode(true);

	const int port = API->getDefaultPort();

	NetAddr broadcastAddr;
	broadcastAddr.CreateBroadcastAddr(port);

	message.setDescriptor(m_descriptor);
	API->sendNetMessage(message, broadcastAddr);

	API->setBroadcastMode(false);
}

void NetObject::SendReplicaMessageToHost(NetMessage& message) const
{
	message.setDescriptor(m_descriptor);
	SendUnicastMessage(message, NetObjectAPI::GetInstance()->getHostAddress());
}

void NetObject::RegisterMessageHandler(std::function<void(NetMessage&, NetAddr const&)> handler)
{
	m_messageHandler = std::move(handler);
}

void NetObject::SetOnReplicaAddedCallback(std::function<void(NetAddr const&)> callback)
{
	m_replicaAddedCallback = std::move(callback);
}

void NetObject::SetOnReplicaLeftCallback(std::function<void(NetAddr const&)> callback)
{
	m_replicaLeftCallback = std::move(callback);
}

void NetObject::SetUpdateFunction(std::function<void()> newUpdate)
{
	m_update = std::move(newUpdate);
}

void NetObject::ReceiveMessage(NetMessage& message, NetAddr const& addr) const
{
	m_messageHandler(message, addr);
}

void NetObject::Update() const
{
	m_update();
}

void NetObject::ReplicaLeftCallback(NetAddr const& addr) const
{
	m_replicaLeftCallback(addr);
}

void NetObject::ReplicaAddedCallback(NetAddr const& addr) const
{
	m_replicaAddedCallback(addr);
}
