#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::m_instance = nullptr;
bool ResourceManager::m_preloadFinished = false;

Sprite * ResourceManager::getResource(Sprites name)
{
	for (const auto &p : m_Resources)
	{
		if (p.first == name)
		{
			return p.second;
		}
	}
	return nullptr;
}

ResourceManager::ResourceManager()
{
	loadResource(Sprites::WHITE, "Data/Images/white.png");
	loadResource(Sprites::LOADING, "Data/Images/loading.png");
}

void ResourceManager::preload()
{
	Game::g_pGame->setGameState(GameState::LOADING);
	std::thread thr([this] {loadResources(); });
	thr.detach();
}

Sprites ResourceManager::getKeyOfSpriteByPath(std::string const& str)
{
	for (auto const& p : m_Resources)
	{
		assert(p.second != nullptr);
		if (p.second->GetName() == str)
		{
			return p.first;
		}
	}
	return Sprites::WHITE;
}

bool ResourceManager::preloadFinished()
{
	return m_preloadFinished;
}

void ResourceManager::loadResources()
{
	//loadResource(Sprites::WHITE, "Data/Images/white.png");
	//loadResource(Sprites::LOADING, "Data/Images/loading.png");

	m_preloadFinished = true;
}

void ResourceManager::loadResource(Sprites name, std::string const& path)
{
	m_Resources.emplace(std::make_pair(name, new Sprite(path.c_str())));
}

ResourceManager::~ResourceManager()
{
	for (auto it = m_Resources.begin(); it != m_Resources.end();) {
		delete it->second;
		it = m_Resources.erase(it);
	}
	m_Resources.clear();
}

ResourceManager* ResourceManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager();
	}

	return m_instance;
}

void ResourceManager::deleteInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
