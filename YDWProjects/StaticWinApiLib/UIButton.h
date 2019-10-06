#pragma once
#include"Physics.h"
#include"BitMap.h"
#include "SpriteRenderer.h"
#include "FontManager.h"

class UIButton
{
protected:
	SpriteRenderer sprite;
	FontManager font;
	int			x;
	int			y;
	char label[128];
	int	sizeX;
	int	sizeY;
public:
	void Init(int buttonImageNum, int _x, int _y, int _sizeX, int _sizeY, char* _label);
	void Draw(HDC hdc, char* fontdir);
	bool isButtonClick(POINT pt);
};
