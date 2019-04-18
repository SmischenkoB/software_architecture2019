#pragma once

#include "Menu/ButtonContainer.h"

class PauseMenuPage : public MenuItem
{
	using super = MenuItem;
public:
	PauseMenuPage();
	~PauseMenuPage() = default;

private:
	ButtonContainer* m_buttons;
	ButtonContainer* m_toggleGameModeButton;

	long const m_greyColor = 0x80808080;
	
	virtual void OnEnd() override;
	virtual void OnStart() override;

	void initButtons();
};
