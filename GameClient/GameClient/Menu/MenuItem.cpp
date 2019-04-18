#include "stdafx.h"

MenuItem::MenuItem() : Object(nullptr)
{
	text[0] = 0;
	textFormat = DT_LEFT | DT_TOP;
	textColor = 0xFFFFFFFF;
	pFont = Font::g_DefaultSmallFont;
}

void MenuItem::Render()
{
	if (!visible)
		return;

	if (pSprite)
	{
		Vector2D spritePos;
		Vector2D spriteSize;
		float spriteRotation;
		Camera2D::g_pActiveCamera->VirtualScreenToScreenSpace(this, spritePos, spriteSize, spriteRotation);

		// draw the Sprite
		pSprite->Draw(spritePos, spriteSize, spriteRotation, sColor(color).Mask(global.colorMask), spriteTopLeftUV, spriteBottomRightUV);
	}

	if (pText != nullptr || text[0])
	{
		pFont->DrawText(pText != nullptr ? pText : text, this, textFormat, sColor(textColor).Mask(global.colorMask));
	}

	Object::Render();
}

void MenuItem::SetText(const char * str)
{
	if (str)
		strcpy_s(text, MAX_TEXT_LEN, str);
	else
		text[0] = 0;
}
