#pragma once

#include "Network/GameStateSynchronizer.h"
#include "HUD/PageManager.h"

class Game final : public Object
{
public:
	static Game* g_pGame;

	Game();
	~Game();

	// Initialize & cleanup the game instance
	void GameInit();
	void GameCleanup();

	// Called each frame to update and draw the game's frame
	void RunFrame(float dt);

	// Update gameplay, static bodies and generate input to physics
	virtual void Update(float dt) override;

	// Update collision and rigidbodies
	void UpdatePhysics(float dt);
	// Update global transforms (global positions for all objects, for rendering purpose)
	virtual void UpdateGlobal() override;

	// Draw one frame
	void Render() override;

	GameState getGameState() const { return m_gameState; }
	void setGameState(GameState state);

private:
	GameState m_gameState;

	std::condition_variable m_preloadCondition;
	std::mutex m_preloadMutex;

	PageManager* m_pageManager;
	GameStateSynchronizer m_gameStateSynchronizer;

};
