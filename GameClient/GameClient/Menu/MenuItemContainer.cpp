#include "stdafx.h"
#include "MenuItemContainer.h"


MenuItemContainer::MenuItemContainer()
{
	pSprite = ResourceManager::getInstance()->getResource(Sprites::WHITE);

	color = m_blackColor;
}

MenuItemContainer::~MenuItemContainer()
{
	clean();
}

void MenuItemContainer::Update(float dt)
{
	super::Update(dt);
}

void MenuItemContainer::addItem(MenuItem * item)
{
	AddChild(item);

	if (m_items.size() != 0)
	{
		MenuItem* lastButton = m_items.back();

		item->position.x = lastButton->position.x;

		item->position.y = lastButton->position.y;
		item->position.y -= lastButton->size.y / 2.f;
	
		for (MenuItem* button : m_items)
		{
			button->position.y += item->size.y / 2.f;
		}
	}

	m_items.push_back(item);

	size.x = m_items.back()->size.x;
	size.y = m_items.back()->size.y * m_items.size();

}

void MenuItemContainer::clean()
{
	while (pChildren)
	{
		delete pChildren;
	}

	m_items.clear();

	size = 0.f;
}
