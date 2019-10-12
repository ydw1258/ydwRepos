#include "BitMap.h"

BitMap::BitMap() {}
BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, int cx, int cy, int r, int g, int b)
{
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, cx, cy);
	hOldBitMap = (HBITMAP)SelectObject(memDC, hBitmap);

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	bitmapSize.cx = bitmap.bmWidth;
	bitmapSize.cy = bitmap.bmHeight;
	
	//흰색으로 초기화
	HPEN pen, oPen;

	//pen = CreatePen(PS_SOLID, width, RGB(r, g, b));
	pen = CreatePen(PS_SOLID, 3000, RGB(r, g, b));

	oPen = (HPEN)SelectObject(GetmemDC(), pen);

	MoveToEx(GetmemDC(),0, 0, NULL);
	LineTo(GetmemDC(), 1, 1);

	SelectObject(GetmemDC(), oPen);
	DeleteObject(pen);
}

void BitMap::Init(HDC hdc, LPCSTR path)
{
	memDC = CreateCompatibleDC(hdc);
	//m_hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(id));
	hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	hOldBitMap = (HBITMAP)SelectObject(memDC, hBitmap); //memDC에 그림넣고, Old는 릴리즈에 

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	bitmapSize.cx = bitmap.bmWidth;
	bitmapSize.cy = bitmap.bmHeight;
}

void BitMap::Draw(HDC backBuffer, int x, int y, int cx, int cy)
{
	//bitmap
	// 그릴곳, 가져올 곳
	TransparentBlt(backBuffer, x, y, cx, cy, memDC, 0, 0, bitmapSize.cx, bitmapSize.cy, RGB(255, 0, 255));
}

void BitMap::Release()
{
	SelectObject(memDC, hOldBitMap);
	DeleteObject(hBitmap); //select된상태에서는 bitmap을 delete못해서 old를 사용.
	DeleteDC(memDC);
}

HDC BitMap::GetmemDC()
{
	return memDC;
}

BitMap* BitMap::Create(HDC hdc, int sizeX, int sizeY)
{
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, sizeX, sizeY);
	hOldBitMap = (HBITMAP)SelectObject(memDC, hBitmap);

	return this;
}
