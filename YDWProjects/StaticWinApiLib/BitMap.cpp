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
	
	//������� �ʱ�ȭ
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
	hOldBitMap = (HBITMAP)SelectObject(memDC, hBitmap); //memDC�� �׸��ְ�, Old�� ����� 

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
