#pragma once

#include "Menu/ButtonContainer.h"
// -------------------------------------------------------------------------------------
// This is the menu page that appears when the game starts
// It doesn't have any functionality in itself, and acts as an overlay
// Te Game handles showing and hiding this menu page
// -------------------------------------------------------------------------------------

class StartMenuPage : public MenuItem
{
	using super = MenuItem;
public:
	StartMenuPage();

private:

	ButtonContainer* m_buttons;

	void initButtons() const;
};
