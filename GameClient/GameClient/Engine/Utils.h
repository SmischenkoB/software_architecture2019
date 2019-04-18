#pragma once

// -------------------------------------------------------------------------------------
// This file contains some useful functions
// Note: add hear other useful functions that don't belong to any objects in particular
// -------------------------------------------------------------------------------------

void InitRand();
float Rand01();
float Rand(float min, float max);
float Lerp(float a, float b, float time);
int Lerp(int a, int b, float time);
Vector2D Lerp(Vector2D a, Vector2D b, float time);
long LerpColor(long a, long b, float time);

struct sColor
{
	sColor(unsigned char a, unsigned char r, unsigned char g, unsigned char b) { A = a; R = r; G = g; B = b; }
	sColor(long color) { A = color >> 24; R = (color >> 16) & 0xFF; G = (color >> 8) & 0xFF; B = color & 0xFF; }

	long GetValue() { return (A << 24) | (R << 16) | (G << 8) | B; }
	long Mask (long mask)
	{
		sColor m(mask);
		m.A = (int)((m.A / 255.f * A / 255.f) * 255);
		m.R = (int)((m.R / 255.f * R / 255.f) * 255);
		m.G = (int)((m.G / 255.f * G / 255.f) * 255);
		m.B = (int)((m.B / 255.f * B / 255.f) * 255);
		return m.GetValue();
	}
	long MaskAlpha(unsigned char mask)
	{
		sColor m(mask, R, G, B);
		m.A = (int)((m.A / 255.f * A / 255.f) * 255);
		return m.GetValue();
	}

	unsigned char A;
	unsigned char R;
	unsigned char G;
	unsigned char B;
};
