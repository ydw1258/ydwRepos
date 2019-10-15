#pragma once

#include <Windows.h>

class BitMap;
class NumberSprite
{
public:
	enum NS_ALIGN
	{
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

private:
	BitMap* bitmap;
	int width;
	int height;
	int sizeX;
	int sizeY;

public:
	NumberSprite();
	~NumberSprite();

	int Init(BitMap* _bitmap, int _width, int _height, int _sizeX, int _sizeY);
	void Render(HDC hdc, int x, int y, int value, int padding = 0, int spacing = 0, NS_ALIGN align = ALIGN_LEFT,
		float scaleX = 1, float scaleY = 1);
};

