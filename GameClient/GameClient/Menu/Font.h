#pragma once

// -------------------------------------------------------------------------------------
// Class used to draw texts on the screen.
// The provided positions are in Virtual Screen Space.
// For your convenince, 2 default fonts (one small and one large) are created by the Engine at initialization.
// 
// Note: If needed, you can create your own fonts and use them in the game.
// -------------------------------------------------------------------------------------

class Font
{
public:
	static Font* g_DefaultSmallFont;
	static Font* g_DefaultLargeFont;

	Font (const WCHAR fontName[LF_FACESIZE], INT height = 20, UINT weight = FW_NORMAL);
	~Font ();

	static void CreateDefaultFonts();

	Vector2D DrawText(const char* text, const Object * pObject, DWORD format = DT_LEFT | DT_TOP, unsigned long color = 0xFFFFFFFF) { return DrawText(text, pObject->global.position, pObject->size, format, color); }
	Vector2D DrawText(const char* text, const Vector2D& pos, const Vector2D& rectSize, DWORD format = DT_LEFT | DT_TOP, unsigned long color = 0xFFFFFFFF);
	Vector2D GetTextSize(const char* text, const Object * pObject, DWORD format = DT_LEFT | DT_TOP) { return DrawText(text, pObject->global.position, pObject->size, format | DT_CALCRECT); }
	Vector2D GetTextSize(const char* text, const Vector2D& pos, const Vector2D& rectSize, DWORD format = DT_LEFT | DT_TOP) { return DrawText(text, pos, rectSize, format | DT_CALCRECT); }
	float GetFontHeight();

protected:
	ID3DXFont *pFont = NULL;
	D3DXFONT_DESC fontDesc;
};
