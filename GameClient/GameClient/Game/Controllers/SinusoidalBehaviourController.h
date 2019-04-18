#pragma once

#include "stdafx.h"

template <class T>
class SinusoidalBehaviourController : public Controller
{
public:
	SinusoidalBehaviourController(T& var, float period, float proportion)
		: m_variable(var)
		, m_period(period)
		, m_proportion(proportion)
	{
		m_value = m_variable;
	}

	void init(float period, float proportion)
	{
		m_period = period;
		m_proportion = proportion;
	}

	~SinusoidalBehaviourController() override = default;

	virtual void Update(float dt) override
	{
		m_timer += dt;

		m_variable = m_value * (fabs(sinf(2.f * M_PI / m_period * m_timer)) * (1.f - m_proportion) + m_proportion);
	}

private:

	T& m_variable;
	T m_value;
	float m_period;

	float m_proportion;
	float m_timer = 0.f;
};