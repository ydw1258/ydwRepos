#include "NumberSprite.h"
#include "BitMap.h"
#include <vector>

NumberSprite::NumberSprite()
{
}

NumberSprite::~NumberSprite()
{
}

int NumberSprite::Init(BitMap * _bitmap, int _width, int _height, int _sizeX, int _sizeY)
{
	bitmap = _bitmap;
	width = _width;
	height = _height;
	sizeX = _sizeX;
	sizeY = _sizeY;

	return 0;
}

void NumberSprite::Render(HDC hdc, int x, int y, int value, int padding, int spacing, NS_ALIGN align
	,float scaleX, float scaleY)
{
	vector<int> digitVec;

	while (value > 0)
	{
		int digit = value % 10;
		digitVec.push_back(digit);
		value /= 10;
	}

	while (digitVec.size() < padding)
	{
		digitVec.push_back(0);
	}

	int left = x;
	switch (align)
	{
	case ALIGN_LEFT:
		left += (digitVec.size() * (width + spacing)) * scaleX;
		break;
	case ALIGN_CENTER:
		left += (digitVec.size() * (width + spacing) / 2) * scaleX;
		break;
	case ALIGN_RIGHT:
		break;
	default:
		break;
	}

	for (int i = 0; i < digitVec.size(); i++)
	{
		int tempX = (digitVec[i] % sizeX) * width;
		int tempY = (digitVec[i] / sizeX) * height;

		bitmap->DrawTransparent(
			hdc,
			left - i * (width + spacing) * scaleX,
			y,
			tempX, tempY,
			width, height,
			scaleX, scaleY);
	}
}