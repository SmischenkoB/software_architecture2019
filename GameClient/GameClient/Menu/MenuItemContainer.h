#pragma once

class MenuItemContainer : public MenuItem
{
	using super = MenuItem;

public:
	MenuItemContainer();
	~MenuItemContainer();

	virtual void Update(float dt) override;

	virtual void addItem(MenuItem* item);

	virtual void clean();

protected:
	std::vector<MenuItem*> m_items;

	long const m_blackColor = 0x80000000;

};

