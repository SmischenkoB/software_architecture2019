#pragma once

enum class ColorChanel
{
	Red = 0,
	Green = 8,
	Blue = 16,
	Alpha = 24
};

class ColorFlowController : public Controller
{
	using super = Controller;

public:
	ColorFlowController(ColorChanel chanel = ColorChanel::Alpha);
	~ColorFlowController();

	void Update(float dt) override;

private:

	long m_speedCoefficient;
	ColorChanel m_chanel;

	float getChanel(ColorChanel chanel) const;
	void setChanel(ColorChanel eChanel, float alpha);


};

