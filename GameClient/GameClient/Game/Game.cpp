#include "stdafx.h" 
#include <IO/InputProxy.h>
#include <sstream>

Game* Game::g_pGame = nullptr;

void GameRender()
{
	if (Game::g_pGame)
		Game::g_pGame->Render();
}

void GameDebugRender()
{
	if (Game::g_pGame)
		Game::g_pGame->DebugRender();
}

Game::Game() : Object()
{
	if (!g_pGame)
		g_pGame = this;

	ObjectPool::getInstance();
	m_pageManager = new PageManager();

	ResourceManager::getInstance()->preload();
}

Game::~Game()
{
	GameCleanup();

	ResourceManager::deleteInstance();
	ObjectPool::deleteInstance();

	if (auto networkAPI = NetObjectAPI::GetInstance())
	{
		networkAPI->Shutdown();
	}
}

void Game::GameInit()
{

}

void Game::GameCleanup()
{
	delete m_pageManager;
}

void Game::RunFrame(float dt)
{
	// Then update gameplay & generate input for the physics
	Update(dt);

	// Then update physics (collisions and rigidbodies)
	UpdatePhysics(dt);

	// Prepare for rendering (drawing) and render
	UpdateGlobal();
	Engine::EngineRender(GameRender, GameDebugRender);
}

void Game::Update(float dt)
{
	if (auto networkAPI = NetObjectAPI::GetInstance())
	{
		networkAPI->Update(dt);
	}

	if (g_pGame->m_gameState == GameState::LOADING)
	{
		std::unique_lock <std::mutex> lk(m_preloadMutex);

		if (m_preloadCondition.wait_for(lk, std::chrono::microseconds(3), [] {return ResourceManager::preloadFinished(); }))
		{
			// initialize object pool after resource manager
			setGameState(GameState::STARTING);
			GameInit();
		}
	}

	m_pageManager->Update(dt);

	// Show / hide debug output on '`' press
	if (Input::IsKeyPressed(VK_OEM_3))
	{
		Engine::g_bShowDebugOutput = !Engine::g_bShowDebugOutput;
	}

	Object::Update(dt);
}

void Game::UpdatePhysics(float dt)
{
}

void Game::UpdateGlobal()
{
	Object::UpdateGlobal();

	m_pageManager->UpdateGlobal();

}

void Game::Render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	Object::Render();

	//	Write code of objects rendering HERE
	m_pageManager->Render();

	
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Game::setGameState(GameState state)
{
	if (m_gameState != state)
	{
		m_gameState = state;
		m_pageManager->changeCurrentPage(state);
		m_gameStateSynchronizer.synchronizeGameState();
	}
}