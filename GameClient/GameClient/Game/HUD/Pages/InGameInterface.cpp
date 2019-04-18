#include "stdafx.h"
#include "InGameInterface.h"

#include <Game/Network/Messages/TimeSynchroniationMessage.h>

namespace
{
	Vector2D const m_timerSize = { 200.f, 30.f };
	Vector2D const m_timerOffset = { 0.f, 260.f };
	Vector2D const m_weaponDisplayerOffset = { -530.f, 0.f };
}

InGameInterface::InGameInterface()
{
	Deactivate();

	m_timer = new MenuGameTimer();
	m_timer->size = m_timerSize;

	m_timerDisplayer = new MenuItemContainer();
	m_timerDisplayer->addItem(m_timer);

	m_timerDisplayer->position += m_timerOffset;

	AddChild(m_timerDisplayer);
}

void InGameInterface::Update(float dt)
{
	if (!active)
	{
		return;
	}

	super::Update(dt);
}

void InGameInterface::OnEnd()
{
	Deactivate();

	m_timer = new MenuGameTimer();
	m_timer->size = m_timerSize;
	m_timerDisplayer->addItem(m_timer);

}
