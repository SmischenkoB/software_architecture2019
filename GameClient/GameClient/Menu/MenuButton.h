#pragma once

class MenuButton : public MenuItem
{
	using super = MenuItem;
public:
	MenuButton(std::function<void()> action = [] {});
	~MenuButton();

	virtual void Update(float dt) override;

	bool isSelected() const { return m_isSelected; }

	void select() { m_isSelected = true; }
	void deselect() { m_isSelected = false; }

	void setColor(long hue) { m_selectedColor = hue; }

	void onClick() { m_onClick(); }

	void setOnClick(std::function<void()> func) { m_onClick = func; }

private:
	bool m_isSelected = false;

	long m_selectedColor = 0x80a0a0a0;

	Vector2D const m_ButtonSizeProportions = { 6, 12 };

	std::function<void()> m_onClick;

};

