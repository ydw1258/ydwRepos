#include "BitMap.h"

BitMap::BitMap() {}
BitMap::~BitMap()
{
	Release();
}

void BitMap::Init(HDC hdc, HINSTANCE hInst, LPCSTR path)
{
	memDC = CreateCompatibleDC(hdc);
	//m_hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(id));
	hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	hOldBitMap = (HBITMAP)SelectObject(memDC, hBitmap); //memDC�� �׸��ĹھƼ�, Old�� ����� 

	BITMAP bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	bitmapSize.cx = bitmap.bmWidth;
	bitmapSize.cy = bitmap.bmHeight;
}

void BitMap::Draw(HDC backBuffer, int x, int y, int cx, int cy)
{
	//bitmap
	// �׸���, ������ ��
	TransparentBlt(backBuffer, x, y, cx, cy, memDC, 0, 0, bitmapSize.cx, bitmapSize.cy, RGB(255, 0, 255));
}

void BitMap::Release()
{
	SelectObject(memDC, hOldBitMap);
	DeleteObject(hBitmap); //select�Ȼ��¿����� bitmap�� delete���ؼ� old�� ���.
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
