#pragma once

class MenuGameTimer : public MenuItem
{
	using super = MenuItem;
public:

	std::string timerText = "0 :: 0 :: 0";

	MenuGameTimer();
	~MenuGameTimer() = default;

	virtual void Update(float dt) override;

	float getSeconds() const { return m_seconds; }
	void setSeconds(float val) { m_seconds = val; }

private:
	float m_seconds = 0.f;
};

