#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class FontManager
{
private:
	HFONT myFont;
	HFONT oldFont;
	int fontOffsetX = 0;
	int fontOffsetY = 0;
public:
	void Init();
	void Draw(int num, int x, int y, int scrollSpeedX = 0, int scrollSpeedY = 0);
	void Draw(const char* str ,int x, int y, int scrollSpeedX = 0, int scrollSpeedY = 0);
	void fontOffsetResetX();
	void fontOffsetResetY();
	void Release();
	
};