#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class FontManager
{
private:
	HFONT myFont;
	HFONT oldFont;
public:
	static void Init();
	
	void Draw(HDC hdc, string str, int fontSize, int x, int y, string fontDirName, COLORREF RGB);
	void Draw(HDC hdc, int num, int fontSize, int x, int y, string fontDirName, COLORREF RGB);
};