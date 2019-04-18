#pragma once

class AnimationController : public Controller
{

public:

	AnimationController(int numSprite = 1, int wNumSprite = 1, int hNumSprite = 1, float speed = 2.f);
	~AnimationController();

	void Init(int amountOfSprites, int amountOfHorizontalSprites, int amountOfVerticalSprites, float animationSpeed = 2.f);
	virtual void Update(float dt) override;
	
	int getAnimationSpriteID() const { return m_animationSpriteID; }
	void setAnimationSpriteID(int val) { m_animationSpriteID = val; }

	int getNumberOfSprites() const { return m_numberOfSprites; }
private:

	float m_verticalStep;
	float m_horisontalStep;

	float m_spriteDeltaTime;
	float m_speed;
	
	int	m_numberOfVerticalSprites;
	int	m_numberOfHorizontalSprites;
	
	int	m_animationSpriteID;
	int	m_numberOfSprites;
};
