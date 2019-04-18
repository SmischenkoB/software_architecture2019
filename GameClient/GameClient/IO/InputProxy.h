#pragma once
#include <IO/Input.h>

class InputProxy final : public Object, Input
{
public:
	static bool IsKeyPressed(int key);
	void Update(float dt) override;
};
