#include "BitMap.h"
//#include "resource.h"

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, int x, int y)
{
	hMemDC = CreateCompatibleDC(hdc);
	hBitMap = CreateCompatibleBitmap(hdc, x, y);
	hOldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);

	BITMAP bitmap;
	GetObject(hBitMap, sizeof(bitmap), &bitmap);
	size.cx = bitmap.bmWidth;
	size.cy = bitmap.bmHeight;
}

void BitMap::Init(HDC hdc, HINSTANCE hInst, LPCWSTR path)
{
	hMemDC = CreateCompatibleDC(hdc);
	//hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(id));
	hBitMap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP,
		0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	hOldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);

	BITMAP bitmap;
	GetObject(hBitMap, sizeof(bitmap), &bitmap);
	size.cx = bitmap.bmWidth;
	size.cy = bitmap.bmHeight;
}

void BitMap::Draw(HDC _hdc, int _x, int _y)
{
	BitBlt(_hdc, _x, _y, _x + size.cx, _y + size.cy, hMemDC, 0, 0, SRCCOPY);
}

void BitMap::Draw(HDC _hdc, int _x, int _y, float _scaleX, float _scaleY)
{
	BitBlt(_hdc, _x, _y, _x + (size.cx * _scaleX), _y + (size.cy*_scaleY), hMemDC, 0, 0, SRCCOPY);
}

void BitMap::DrawTransparent(HDC _hdc, int _x, int _y)
{
	TransparentBlt(_hdc, _x, _y, size.cx, size.cy,
		hMemDC, 0, 0, size.cx, size.cy, RGB(255, 0, 255));
}

void BitMap::DrawTransparent(HDC _hdc, int _x, int _y, int _w, int _h)
{
	TransparentBlt(_hdc, _x, _y, _w, _h,
		hMemDC, 0, 0, size.cx, size.cy, RGB(255, 0, 255));
}

void BitMap::DrawTransparent(HDC _hdc, int _x, int _y, int _sx, int _sy, int _sw, int _sh, float _scaleX, float _scaleY)
{
	TransparentBlt(_hdc, _x, _y, _sw * _scaleX, _sh * _scaleY,
		hMemDC, _sx, _sy, _sw, _sh, RGB(255,0,255));
}

void BitMap::Release()
{
	SelectObject(hMemDC, hOldBitMap);
	DeleteObject(hBitMap);
	DeleteDC(hMemDC);
}