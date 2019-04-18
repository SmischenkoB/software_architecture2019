#pragma once

// -------------------------------------------------------------------------------------
// Base class from that represents a 2D spatial transformation (position, rotation and scale)
// -------------------------------------------------------------------------------------

class Transform2D
{
public:
	Transform2D(Vector2D _position = { 0 , 0 }, float _rotation = 0, Vector2D _scale = { 1, 1 })
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}

	virtual void Reset()
	{
		position = 0;
		rotation = 0;
		scale = 1;
	}

	Vector2D		position;
	float			rotation;
	Vector2D		scale;
};
