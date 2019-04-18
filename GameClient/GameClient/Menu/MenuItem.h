#pragma once

#define MAX_TEXT_LEN 256

// -------------------------------------------------------------------------------------
// Class that defines the basic Menu Item.
// Beside the image / sprite inherited from the Object, it also has a text.
// It adds the functionality of Rendering (the image & the text) in the Virtual Screen Space.
// It can be derrived to extend its functionality - see the EditBox class
// -------------------------------------------------------------------------------------

class MenuItem : public Object
{
public:
	MenuItem();

	virtual void Render();

	void SetText(const char* str);

	virtual Type GetType() { return Type::MENUITEM; }
	virtual bool IsTypeOf(Type type) { return (type == Type::MENUITEM) || Object::IsTypeOf(type); }

	unsigned long	textColor;
	DWORD			textFormat;
	Font*			pFont;

	char			text[MAX_TEXT_LEN];
	char *			pText;
};
