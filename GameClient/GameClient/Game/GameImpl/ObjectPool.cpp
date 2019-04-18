#include "stdafx.h"
#include "ObjectPool.h"


ObjectPool* ObjectPool::m_instance = nullptr;

ObjectPool::ObjectPool()
{
	// Place for reserving and initializing vectors of objects
}


ObjectPool::~ObjectPool()
{
	// Place for deleting objects form array and clearing

	//for each in vector : delete e, vector.clear

}

ObjectPool* ObjectPool::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ObjectPool();
	}

	return m_instance;
}

void ObjectPool::deleteInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

// EXAMPLE ::

//Object* ObjectPool::getFreeObjectInstance() const
//{
//	for (Object* object : m_objectVector)
//	{
//		if (!object->visible)
//		{
//			return object;
//		}
//	}
//	return nullptr;
//}
