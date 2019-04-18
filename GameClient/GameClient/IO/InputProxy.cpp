#include "stdafx.h"
#include "InputProxy.h"

bool InputProxy::IsKeyPressed(int key)
{
	return (keyboardState[key] & 0x80) && !(previousKeyboardState[key] & 0x80);
}

void InputProxy::Update(float dt)
{
	// update the keyboard state
	memcpy_s(previousKeyboardState, 256, keyboardState, 256);
	GetKeyboardState(keyboardState);

	Object::Update(dt);
}
