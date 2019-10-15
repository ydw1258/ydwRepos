#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class BitMap
{
private:
	HDC hMemDC;
	HBITMAP hBitMap;
	HBITMAP hOldBitMap;

	SIZE size;

public:
	HDC getDC() { return hMemDC; }

public:
	BitMap();
	~BitMap();

	void Init(HDC hdc, int x, int y);
	void Init(HDC hdc, HINSTANCE hInst, LPCWSTR path);
	void Draw(HDC hdc, int x, int y);
	void Draw(HDC hdc, int x, int y, float _scaleX, float _scaleY);
	void DrawTransparent(HDC hdc, int x, int y);
	void DrawTransparent(HDC _hdc, int _x, int _y, int _w, int _h);
	void DrawTransparent(HDC hdc, int x, int y, 
		int _sx, int _sy, int _sw, int _sh, float _scaleX = 1, float _scaleY = 1);
	void Release();
};

