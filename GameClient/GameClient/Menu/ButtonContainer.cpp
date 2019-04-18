#include "stdafx.h"
#include "ButtonContainer.h"


ButtonContainer::ButtonContainer()
{
	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);

	color = m_blackColor;
}

ButtonContainer::~ButtonContainer()
{
	clear();
}

void ButtonContainer::Update(float dt)
{
	super::Update(dt);

	updateMousePosition();

	if (checkMousePosition(this))
	{
		for (MenuButton* button : m_buttons)
		{
			if (checkMousePosition(button))
			{
				button->select();
			}
			else
			{
				button->deselect();
			}
		}

		if (Input::IsKeyPressed(VK_LBUTTON))
		{
			for (MenuButton* button : m_buttons)
			{
				if (button->isSelected())
				{
					button->onClick();
				}
			}
		}
	}
	else
	{

		if (Input::IsKeyPressed(VK_UP) || Input::IsKeyPressed('W') || Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_DPAD_UP))
		{
			if (std::none_of(m_buttons.begin(), m_buttons.end(), [](MenuButton* button) { return button->isSelected(); }))
			{
				m_buttons.front()->select();
			}

			for (size_t i = 0; i < m_buttons.size(); i++)
			{
				if (m_buttons[i]->isSelected())
				{
					m_buttons[i]->deselect();

					if (i == 0)
					{
						i = m_buttons.size() - 1;
						m_buttons[i % m_buttons.size()]->select();
					}
					else
					{
						m_buttons[--i % m_buttons.size()]->select();
					}


				}
			}
		}

		if (Input::IsKeyPressed(VK_DOWN) || Input::IsKeyPressed('S') || Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			if (std::none_of(m_buttons.begin(), m_buttons.end(), [](MenuButton* button) { return button->isSelected(); }))
			{
				m_buttons.back()->select();
			}

			for (size_t i = 0; i < m_buttons.size(); i++)
			{
				if (m_buttons[i]->isSelected())
				{
					m_buttons[i]->deselect();
					m_buttons[(++i) % m_buttons.size()]->select();
				}
			}
		}

		if (Input::IsKeyPressed(VK_RETURN) || Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_A))
		{
			for (MenuButton* button : m_buttons)
			{
				if (button->isSelected())
				{
					button->onClick();
				}
			}
		}

	}

	if (!checkMousePosition(this) && mouseIsMoving())
	{
		std::for_each(m_buttons.begin(), m_buttons.end(), [](MenuButton* button) { button->deselect(); });
	}
	
}

void ButtonContainer::addButton(MenuButton * button)
{
	AddChild(button);

	if (m_buttons.size() != 0)
	{
		for (MenuButton* button : m_buttons)
		{
			button->position.y += button->size.y / 2.f;
		}
		MenuButton* lastButton = m_buttons.back();

		button->position.x = lastButton->position.x;

		button->position.y = lastButton->position.y;
		button->position.y -= lastButton->size.y;
	}

	m_buttons.push_back(button);

	size.x = m_buttons.back()->size.x;
	size.y = m_buttons.back()->size.y * m_buttons.size();
}

void ButtonContainer::resetColor()
{
	color = m_blackColor;
}

void ButtonContainer::clear()
{
	for (MenuButton* button : m_buttons)
	{
		delete button;
	}

	m_buttons.clear();

	size = { 0, 0 };
}

bool ButtonContainer::checkMousePosition(MenuItem* item) const
{
	if (item == this)
	{
		bool mouseIsOnItem = currentVirtualMousePos.x < item->position.x + item->size.x / 2.f;
		mouseIsOnItem &= currentVirtualMousePos.x > item->position.x - item->size.x / 2.f;

		mouseIsOnItem &= currentVirtualMousePos.y < item->position.y + item->size.y / 2.f;
		mouseIsOnItem &= currentVirtualMousePos.y > item->position.y - item->size.y / 2.f;

		return mouseIsOnItem;
	}
	else
	{
		bool mouseIsOnItem = currentVirtualMousePos.x < item->position.x + position.x + item->size.x / 2.f;
		mouseIsOnItem &= currentVirtualMousePos.x > item->position.x + position.x - item->size.x / 2.f;

		mouseIsOnItem &= currentVirtualMousePos.y < item->position.y + position.y + item->size.y / 2.f;
		mouseIsOnItem &= currentVirtualMousePos.y > item->position.y + position.y - item->size.y / 2.f;

		return mouseIsOnItem;
	}
}

void ButtonContainer::updateMousePosition()
{
	previousVirtualMousePos = currentVirtualMousePos;
	currentVirtualMousePos = Input::GetMousePosition();
}

bool ButtonContainer::mouseIsMoving() const
{
	bool mouseIsOnItem = previousVirtualMousePos.x != Input::GetMousePosition().x;
	mouseIsOnItem &= previousVirtualMousePos.y != Input::GetMousePosition().y;

	return mouseIsOnItem;
}
