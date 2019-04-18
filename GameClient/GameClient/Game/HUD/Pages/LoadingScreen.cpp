#include "stdafx.h"
#include "LoadingScreen.h"


LoadingScreen::LoadingScreen()
{
	Deactivate();
	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);
	SetText("LOADING");
	size = Camera2D::g_pActiveCamera->virtualScreenSize;
	pFont = Font::g_DefaultLargeFont;
	color = 0xff808080;
	textFormat = DT_CENTER | DT_TOP;
	m_aditionalSprite = ResourceManager::getInstance()->getResource(Sprites::LOADING);
}

void LoadingScreen::Render()
{
	if (!visible)
		return;
	super::Render();

	Vector2D position;
	Camera2D::g_pActiveCamera->VirtualScreenToScreenSpace(global.position, position);
	m_spriteRotation += 0.5f;
	m_aditionalSprite->Draw(position,
		m_aditionalSprite->size, 
		m_spriteRotation, 0xffffffff, 
		spriteTopLeftUV, 
		spriteBottomRightUV);
}
