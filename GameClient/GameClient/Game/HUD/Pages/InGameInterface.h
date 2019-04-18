#pragma once

#include "Game/HUD/Interfaces/MenuGameTimer.h"
#include "Menu/MenuItemContainer.h"

class InGameInterface : public MenuItem
{
	using super = MenuItem;

public:
	InGameInterface();

	virtual void Update(float dt) override;
	virtual void OnEnd() override;

	float getSeconds() const { return m_timer->getSeconds(); }
	void setSeconds(float val) { m_timer->setSeconds(val); }

private:
	MenuGameTimer* m_timer;

	MenuItemContainer* m_timerDisplayer;
};

