#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class FontManager
{
private:
	HFONT myFont;
	HFONT oldFont;
public:
	void Init();
	void Draw(int num, int x, int y, string fontDirName);
	void Draw(string str, int x, int y, string fontDirName);
	void Release();
};