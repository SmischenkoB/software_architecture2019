#include "stdafx.h"
#include "MenuButton.h"

MenuButton::MenuButton(std::function<void()> action)
	: m_onClick(action)
{
	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);
	size = Camera2D::g_pActiveCamera->virtualScreenSize / m_ButtonSizeProportions;

	pFont = new Font(L"Comic Sans MS", 30, FW_BOLD);
	textFormat = DT_CENTER | DT_VCENTER;
}

MenuButton::~MenuButton()
{
	delete pFont;
}

void MenuButton::Update(float dt)
{
	if (!m_isSelected)
	{
		color = 0x00000000;
	}
	else
	{
		color = m_selectedColor;
	}

	super::Update(dt);
}