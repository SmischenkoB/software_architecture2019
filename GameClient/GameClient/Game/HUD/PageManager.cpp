#include "stdafx.h"
#include "PageManager.h"


PageManager::PageManager()
{
	m_pages.emplace_back(GameState::LOADING, new LoadingScreen());
	m_pages.emplace_back(GameState::STARTING, new StartMenuPage());
	m_pages.emplace_back(GameState::IN_GAME, new InGameInterface());
	m_pages.emplace_back(GameState::PAUSED, new PauseMenuPage());

	for (auto i : m_pages)
	{
		i.second->Deactivate();
		AddChild(i.second);
	}
}


void PageManager::changeCurrentPage(GameState newGameState)
{
	auto pair = *std::find_if(m_pages.begin(), m_pages.end(),
		[newGameState](std::pair<GameState, MenuItem*> pair)
	{
		return pair.first == newGameState;
	});

	for (auto i : m_pages)
	{
		if (i.second->active)
		{
			i.second->Deactivate();
		}
	}
	pair.second->Start();
}

InGameInterface & PageManager::getInGameInterface() const
{
	auto pair = *std::find_if(m_pages.begin(), m_pages.end(),
		[](std::pair<GameState, MenuItem*> pair)
	{
		return pair.first == GameState::IN_GAME;
	});
	auto ret = pair.second;
	return *static_cast<InGameInterface*>(ret);
}
