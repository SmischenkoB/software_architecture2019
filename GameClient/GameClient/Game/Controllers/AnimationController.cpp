#include "stdafx.h"
#include "AnimationController.h"

AnimationController::AnimationController(int numberOfSprites, int numberOfHorizontalSprites, int numberOfVerticalSprites, float numberSpeed)
{
	Init(numberOfSprites, numberOfHorizontalSprites, numberOfVerticalSprites, numberSpeed);
}

void AnimationController::Init(int numberOfSprites, int numberOfHorizontalSprites, int numberOfVerticalSprites, float animationSpeed)
{
	m_verticalStep = 1.f / numberOfVerticalSprites;
	m_horisontalStep = 1.f / numberOfHorizontalSprites;
	m_numberOfVerticalSprites = numberOfVerticalSprites;
	m_numberOfHorizontalSprites = numberOfHorizontalSprites;
	m_numberOfSprites = numberOfSprites;
	m_animationSpriteID = 0;
	m_spriteDeltaTime = 0.f;
	m_speed = animationSpeed == 0.f ? 1.f : animationSpeed;
}

AnimationController::~AnimationController()
{
	Remove();
}

void AnimationController::Update(float dt)
{
	m_spriteDeltaTime += dt;


	if (object->visible && m_spriteDeltaTime > dt * FRAMES_PER_SECOND / m_numberOfSprites * (2.f / m_speed))
	{
		if (m_animationSpriteID + 1 < m_numberOfSprites)
		{
			m_animationSpriteID = (m_animationSpriteID + 1) % m_numberOfSprites;
		}
		m_spriteDeltaTime = 0;
	}

	object->spriteTopLeftUV.y = (m_animationSpriteID / m_numberOfVerticalSprites) * m_horisontalStep;
	object->spriteTopLeftUV.x = (m_animationSpriteID % m_numberOfVerticalSprites) * m_verticalStep;

	object->spriteBottomRightUV.x = object->spriteTopLeftUV.x + m_verticalStep;
	object->spriteBottomRightUV.y = object->spriteTopLeftUV.y + m_horisontalStep;

}
