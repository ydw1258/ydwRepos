#include "AnimClip.h"
#include "BitMap.h"

AnimClip::AnimClip()
{
}

AnimClip::~AnimClip()
{
}

void AnimClip::Init(BitMap * _bitmap, int _width, int _height, int _countX, int _length)
{
	bitmap = _bitmap;

	width = _width;
	height = _height;

	countX = _countX;
	length = _length;

	startIdx = 0;
	endIdx = length-1;
}

void AnimClip::Init(BitMap * _bitmap, int _width, int _height, int _countX, int _length,
	int _startIdx, int _endIdx)
{
	bitmap = _bitmap;

	width = _width;
	height = _height;

	countX = _countX;
	length = _length;

	startIdx = _startIdx;
	endIdx = _endIdx;
}

void AnimClip::Render(HDC hdc, int x, int y, int frame)
{
	bitmap->DrawTransparent(hdc, x, y,
		(frame%countX)*width, (frame / countX)*height,
		width, height,
		1.0f, 1.0f);
}
