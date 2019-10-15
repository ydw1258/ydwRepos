#pragma once
#include <Windows.h>

class BitMap
{
private:
	HDC memDC;//hdc ���� �׸��� ��ȭ��, backbuffer �� memDC ��Ʈ�� ��ü ��ȭ������ ����, �ٱ׸��� ����hdc�� �׸�
	HBITMAP hBitmap;
	HBITMAP hOldBitMap;
	SIZE	bitmapSize;
public:
	void Init(HDC hdc, HINSTANCE hInst, LPCSTR path);
	void Draw(HDC backBuffer, int x, int y, int cx, int cy);
	void Release();
	HDC GetmemDC();
	BitMap* Create(HDC hdc, int sizeX, int sizeY);
	

	BitMap();
	~BitMap();
};