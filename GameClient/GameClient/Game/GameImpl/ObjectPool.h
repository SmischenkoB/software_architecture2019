#pragma once

class ObjectPool
{
public:

	static ObjectPool* getInstance();
	static void deleteInstance();
	
	void initEnemyNetObjects();

private:
	ObjectPool();
	~ObjectPool();

	static ObjectPool* m_instance;
	int const m_poolSize = 255;
	unsigned const m_playerPoolSize = 32;
};

