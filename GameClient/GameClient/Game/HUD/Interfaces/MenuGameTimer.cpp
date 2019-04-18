#include "stdafx.h"
#include "MenuGameTimer.h"

MenuGameTimer::MenuGameTimer()
{
	Deactivate();
	pSprite = nullptr;
	SetText(timerText.c_str());

	pFont = Font::g_DefaultLargeFont;
	color = 0x80808080;
	textFormat = DT_CENTER | DT_TOP;
}

void MenuGameTimer::Update(float dt)
{
	if (Game::g_pGame->getGameState() == GameState::IN_GAME)
	{
		if (!active)
		{
			Activate();
		}
	}
	else
	{
		Deactivate();
	}

	timerText.clear();
	timerText += std::to_string(static_cast<int>(m_seconds) / 3600) + " :: " +
		std::to_string(static_cast<int>(m_seconds) % 3600 / 60) + " :: " +
		std::to_string(static_cast<int>(m_seconds) % 60);

	SetText(timerText.c_str());

	super::Update(dt);
}
