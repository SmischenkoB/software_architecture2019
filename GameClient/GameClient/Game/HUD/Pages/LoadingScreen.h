#pragma once
#include "Menu\MenuItem.h"

class LoadingScreen : public MenuItem
{
	using super = MenuItem;

public:
	LoadingScreen();
	~LoadingScreen() override = default;

	virtual void Render() override;

private:

	Sprite * m_aditionalSprite;
	float m_spriteRotation = 0.f;
};

