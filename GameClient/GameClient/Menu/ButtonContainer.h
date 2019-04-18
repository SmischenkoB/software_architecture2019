#pragma once
#include "MenuButton.h"

class ButtonContainer :	public MenuItem
{
	using super = MenuItem;
public:
	ButtonContainer();
	~ButtonContainer();

	virtual void Update(float dt) override;

	void addButton(MenuButton* button);

	void resetColor();

	void clear();

private:
	std::vector<MenuButton*> m_buttons;
	long const m_blackColor = 0xa0000000;

	Vector2D previousVirtualMousePos = { 0, 0 };
	Vector2D currentVirtualMousePos = { 0, 0 };

	void updateMousePosition();
	bool checkMousePosition(MenuItem* item) const;

	bool mouseIsMoving() const;
};

