#include "BitMap.h"

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, HINSTANCE hInst, LPCSTR path)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	//m_hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(id));
	m_hBitMap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP
		, 0, 0 , LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

	BITMAP bitmap;
	GetObject(m_hBitMap, sizeof(bitmap), &bitmap);
	m_size.cx = bitmap.bmWidth;
	m_size.cy = bitmap.bmHeight;
}

void BitMap::Draw(HDC hdc, int x, int y, int cx, int cy)
{
	BitBlt(hdc, x, y, cx + m_size.cx, cy + m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
}

void BitMap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
