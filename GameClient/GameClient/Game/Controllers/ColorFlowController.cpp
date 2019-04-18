#include "stdafx.h"
#include "ColorFlowController.h"


ColorFlowController::ColorFlowController(ColorChanel chanel)
	:m_chanel(chanel)
{
}

ColorFlowController::~ColorFlowController()
{
}

void ColorFlowController::Update(float dt)
 {
	if (object->ending)
	{
		float const deltaChanel = 0.008f;

		float const currentChanelValue = getChanel(m_chanel);

		if (currentChanelValue - deltaChanel > 0.f)
		{
			setChanel(m_chanel, currentChanelValue - deltaChanel);
		}
		else
		{
			object->End();
		}
	}

	super::Update(dt);
}

float ColorFlowController::getChanel(ColorChanel chanel) const
{
	int const chanelValue = 0xFF & (object->color >> static_cast<unsigned>(chanel));

	return std::fabs(static_cast<float>(chanelValue) / static_cast <float>(0xff));
}

void ColorFlowController::setChanel(ColorChanel eChanel, float value)
{
	unsigned const mask = 0x000000ff;
	unsigned chanel = static_cast<unsigned>(eChanel);

	object->color &= ~(mask << chanel);
	object->color |= (static_cast<unsigned>(0xff * value) << chanel);
}
