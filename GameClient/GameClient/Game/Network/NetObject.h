#pragma once

struct NetObjectDescriptor
{
	int objectType;
	int m_instanceID;

};

static bool operator ==(NetObjectDescriptor const left, NetObjectDescriptor const right)
{
	return left.objectType == right.objectType && left.m_instanceID == right.m_instanceID;
}

namespace std
{
	template<>
	struct hash<NetObjectDescriptor>final
	{
		int operator()(const NetObjectDescriptor& cmp) const noexcept
		{
			return static_cast<int>(hash<int>()(cmp.m_instanceID));
		}
	};
}

class NetMessage;

class NetObject
{
public:
	NetObject(bool const isMaster, NetObjectDescriptor const& descriptor);
	~NetObject();

	NetObject(NetObject const& other) = delete;

	bool IsMaster() const { return m_isMaster; }

	void SendMasterBroadcast(NetMessage& message) const;
	void SendUnicastMessage(NetMessage& message, NetAddr const& addr) const;

	void LANBroadcast(NetMessage& message) const;
	void SendReplicaMessageToHost(NetMessage& message) const;

	void RegisterMessageHandler(std::function<void(NetMessage&, NetAddr const&)> handler);

	void SetOnReplicaAddedCallback(std::function<void(NetAddr const&)> callback);
	void SetOnReplicaLeftCallback(std::function<void(NetAddr const&)> callback);

	void SetUpdateFunction(std::function<void()> newUpdate);

	NetObjectDescriptor getDescriptor() const { return m_descriptor; }

	//This is only for API
	void ReceiveMessage(NetMessage& message, NetAddr const& addr) const;
	void Update() const;
	void ReplicaLeftCallback(NetAddr const&) const;
	void ReplicaAddedCallback(NetAddr const&) const;

private:
	NetObjectDescriptor const m_descriptor;
	bool const m_isMaster;

	std::function<void(NetAddr const&)> m_replicaAddedCallback = [](NetAddr const&) {};
	std::function<void(NetAddr const&)> m_replicaLeftCallback = [](NetAddr const&) {};

	std::function<void(NetMessage&, NetAddr const&)> m_messageHandler;

	std::function<void()> m_update = [] {};

};

