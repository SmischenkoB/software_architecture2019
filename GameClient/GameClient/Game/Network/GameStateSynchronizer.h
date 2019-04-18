#pragma once

class NetObject;

class GameStateSynchronizer : public Object
{
public:
	GameStateSynchronizer() = default;
	~GameStateSynchronizer() = default;
	
	void initNetObject();

	void synchronizeGameState();

private:
	
	NetObject* m_gameStateNetObject;
};

