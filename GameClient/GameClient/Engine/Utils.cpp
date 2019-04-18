#include "stdafx.h"

void InitRand()
{
	time_t t;
	srand((unsigned int)time(&t));
}

float Rand01()
{
	return (float)rand() / (float)RAND_MAX;
}

float Rand(float min, float max)
{
	return min + Rand01() * (max - min);
}

float Lerp(float a, float b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return (b - a) * time + a;
}

int Lerp(int a, int b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return static_cast<int>((b - a) * time) + a;
}

Vector2D Lerp(Vector2D a, Vector2D b, float time)
{
	if (time <= 0) return a;
	if (time >= 1) return b;
	return a + (b - a) * time;
}

long LerpColor(long a, long b, float time)
{
	sColor colorA(a);
	sColor colorB(b);
	sColor ret(0);

	ret.A = Lerp(colorA.A, colorB.A, time);
	ret.R = Lerp(colorA.R, colorB.R, time);
	ret.G = Lerp(colorA.G, colorB.G, time);
	ret.B = Lerp(colorA.B, colorB.B, time);

	return ret.GetValue();
}


