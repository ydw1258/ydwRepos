#pragma once
#include <Windows.h>

enum RESOURCENAME
{
	BLOCK,
	BLOCK00,
	BLOCK01,
	BLOCK02,
	BLOCK03,
	BLOCK04,
	BLOCK05,
	BLOCK06,
	BLOCK07,
	BLOCK08,
	FLAG,
	MINE,
	BACK,
};

class BitMap
{
private:
	HDC m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldBitMap;
	SIZE	m_size;
public:
	void Init(HDC hdc, HINSTANCE hInst, LPCSTR path);
	void Draw(HDC hdc, int x, int y, int cx, int cy);
	void Release();
	BitMap();
	~BitMap();
};

