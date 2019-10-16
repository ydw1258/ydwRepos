#pragma once
#include <Windows.h>

class BitMap
{
private:
	HDC memDC;//hdc 지금 그리는 도화지, backbuffer 에 memDC 비트맵 객체 도화지들을 붙임, 다그리면 메인hdc에 그림
	HBITMAP hBitmap;
	HBITMAP hOldBitMap;
	SIZE	bitmapSize;
public:
	void Init(HDC hdc, int x, int y, int r = 255, int g = 255, int b = 255);
	void Init(HDC hdc, LPCSTR path);
	void Draw(HDC backBuffer, int x, int y, int cx, int cy);
	void Release();
	HDC GetmemDC();
	BitMap* Create(HDC hdc, int sizeX, int sizeY);

	BitMap();
	~BitMap();
};