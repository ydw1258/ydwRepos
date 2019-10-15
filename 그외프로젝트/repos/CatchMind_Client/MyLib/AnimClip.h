#pragma once
#include <Windows.h>

class BitMap;
class AnimClip
{
private:
	BitMap* bitmap;
	int width;
	int height;
	int countX;
	int length;
	int startIdx;
	int endIdx;

public:
	int getStart() { return startIdx; }
	int getEnd() { return endIdx; }

public:
	AnimClip();
	~AnimClip();

	void Init(BitMap* _bitmap, int _width, int _height, int _countX, int _length);
	void Init(BitMap* _bitmap, int _width, int _height, int _countX, int _length,
		int _startIdx, int _endIdx);
	void Render(HDC hdc, int x, int y, int frame);
};

