
#include "stdafx.h"

PauseMenuPage::PauseMenuPage() : super()
{
	Deactivate();

	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);
	SetText("\n\nPAUSED");

	size = Camera2D::g_pActiveCamera->virtualScreenSize;
	pFont = Font::g_DefaultLargeFont;

	color = 0x80808080;
	textFormat = DT_CENTER | DT_TOP;

	m_buttons = new ButtonContainer();
	AddChild(m_buttons);

	initButtons();
}

void PauseMenuPage::initButtons()
{
	
}

void PauseMenuPage::OnEnd()
{
	Game::g_pGame->setGameState(GameState::IN_GAME);
	m_buttons->Deactivate();
	super::OnEnd();
}

void PauseMenuPage::OnStart()
{
	color = m_greyColor;

	m_buttons->Start();
	m_buttons->resetColor();

	SetText("\n\nPAUSED");
}
