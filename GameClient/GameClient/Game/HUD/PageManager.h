#pragma once

#include "Pages/StartMenuPage.h"
#include "Pages/MenuGamePause.h"
#include "Pages/LoadingScreen.h"
#include "Pages/InGameInterface.h"

enum class GameState {
	AUTHENTICATION,
	LOADING,
	STARTING,
	STARTING_LOCAL_MP,
	STARTING_ONLINE_MP,
	IN_LOBBY,
	IN_GAME,
	PAUSED,
	ENDING,
	OVER
};

class PageManager : public UpdateList
{
	using super = Object;
public:
	PageManager();
	~PageManager() = default;
	
	void changeCurrentPage(GameState newGameState);

	InGameInterface& getInGameInterface() const;

private:
	
	std::vector<std::pair<GameState, MenuItem*>> m_pages;
	
};

