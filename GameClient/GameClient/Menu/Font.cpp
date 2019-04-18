#include "stdafx.h"

Font* Font::g_DefaultSmallFont = NULL;
Font* Font::g_DefaultLargeFont = NULL;

Font::Font(const WCHAR fontName[LF_FACESIZE], INT height, UINT weight)
{
	fontDesc = { (INT)(height * Camera2D::g_pActiveCamera->virtualScreenRatio), 0, weight, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, L"Arial" };
	if (fontName)
		wcscpy_s(fontDesc.FaceName, LF_FACESIZE, fontName);
	if (D3DXCreateFontIndirect(Engine::g_pd3dDevice, &fontDesc, &pFont) != D3D_OK)
	{
		pFont = NULL;
	}
}

Font::~Font ()
{
	if (pFont)
		pFont->Release();
}

void Font::CreateDefaultFonts()
{
	if (g_DefaultSmallFont != NULL || g_DefaultLargeFont != NULL)
		return;
	g_DefaultSmallFont = new Font(L"Arial", 15);
	g_DefaultLargeFont = new Font(L"Arial", 30, FW_BOLD);
}

Vector2D Font::DrawText(const char * text, const Vector2D & pos, const Vector2D & rectSize, DWORD format, unsigned long color)
{
	RECT rect;

	Vector2D screenPos;
	Vector2D virtualPos = Vector2D(pos.x - rectSize.x / 2, pos.y + rectSize.y/2);
	Camera2D::g_pActiveCamera->VirtualScreenToScreenSpace(virtualPos, screenPos);
	rect.left = (LONG)screenPos.x;
	rect.top = (LONG)screenPos.y;

	virtualPos.Set(pos.x + rectSize.x / 2, pos.y - rectSize.y / 2);
	Camera2D::g_pActiveCamera->VirtualScreenToScreenSpace(virtualPos, screenPos);
	rect.right = (LONG)screenPos.x;
	rect.bottom = (LONG)screenPos.y;

	pFont->DrawTextA(NULL, text, -1, &rect, format, color);

	if (format & DT_CALCRECT)
		return Vector2D ((rect.right - rect.left) / Camera2D::g_pActiveCamera->virtualScreenRatio, (rect.bottom - rect.top) / Camera2D::g_pActiveCamera->virtualScreenRatio);
	return rectSize;
}

float Font::GetFontHeight()
{
	return fontDesc.Height / Camera2D::g_pActiveCamera->virtualScreenRatio;
}
