#pragma once
#include <Windows.h>
#include<map>
#include<string>
#include"BitMap.h"
using namespace std;

enum IMAGENUM
{
	IMAGENUM_BLACKBACKGROUND,
	IMAGENUM__BLOCK,
	IMAGENUM__BRICK,
	IMAGENUM__BUSH,
	IMAGENUM__EMPTY,
	IMAGENUM__STEEL,
	IMAGENUM_BLOCK,
	IMAGENUM_BRICK,
	IMAGENUM_BUSH,
	IMAGENUM_EMPTY,
	IMAGENUM_STEEL,
	IMAGENUM_WATER,

	IMAGENUM_ROW_1_UP_00,
	IMAGENUM_ROW_1_UP_01,
	IMAGENUM_ROW_1_LEFT_00,
	IMAGENUM_ROW_1_LEFT_01,
	IMAGENUM_ROW_1_RIGHT_00,
	IMAGENUM_ROW_1_RIGHT_01,
	IMAGENUM_ROW_1_DOWN_00,
	IMAGENUM_ROW_1_DOWN_01
};

class ResourceManager
{
private:
	static ResourceManager* mthis;
	BitMap* bitmap;
public:
	static BitMap* backBuffer;

	static ResourceManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ResourceManager();
		return mthis;
	}

	void Release();
	void Init(HDC hdc, HINSTANCE hInstance);
	void Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum);
	ResourceManager();
	~ResourceManager();
};