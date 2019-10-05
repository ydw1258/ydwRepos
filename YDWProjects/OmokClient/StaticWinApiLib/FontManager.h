#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class FontManager
{
private:
	HFONT myFont;
	HFONT oldFont;
public:
	void Draw(string str, int fontSize, int x, int y, string fontDirName, COLORREF RGB);
	void Draw(int num, int fontSize, int x, int y, string fontDirName, COLORREF RGB);
	void Release();
};