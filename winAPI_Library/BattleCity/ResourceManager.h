#pragma once
#include <Windows.h>
#include<map>
#include<string>
#include"BitMap.h"
using namespace std;

enum IMAGENUM
{
	IMAGENUM_EMPTY,
	IMAGENUM_BRICK,
	IMAGENUM_BLOCK,
	IMAGENUM_WATER,
	IMAGENUM_BUSH,
	IMAGENUM_STEEL,
	IMAGENUM_BLACKBACKGROUND,
	/*"BattleResource/__empty.bmp"
	"BattleResource/__brick.bmp",
	"BattleResource/__block.bmp",
	"BattleResource/__water.bmp",
	"BattleResource/__bush.bmp",
	"BattleResource/__steel.bmp", */

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