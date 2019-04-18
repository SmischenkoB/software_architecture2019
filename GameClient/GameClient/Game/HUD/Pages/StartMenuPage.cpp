#include "stdafx.h"

StartMenuPage::StartMenuPage() : super()
{
	Deactivate();

	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);
	size = Camera2D::g_pActiveCamera->virtualScreenSize;

	pFont = Font::g_DefaultLargeFont;

	textFormat = DT_CENTER | DT_VCENTER;

	m_buttons = new ButtonContainer();
	AddChild(m_buttons);

	initButtons();
}

void StartMenuPage::initButtons() const
{

}
