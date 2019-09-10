#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	string fileNames[50] = {"BattleResource/Res/__block.bmp",
							"BattleResource/Res/__brick.bmp",
							"BattleResource/Res/__bush.bmp",
							"BattleResource/Res/__empty.bmp",
							"BattleResource/Res/__steel.bmp",
							"BattleResource/Res/_block.bmp",
							"BattleResource/Res/_brick.bmp",
							"BattleResource/Res/_bush.bmp",
							"BattleResource/Res/_empty.bmp",
							"BattleResource/Res/_steel.bmp",
							"BattleResource/Res/_water.bmp",
	};

	bitmap = new BitMap[50];

	backBuffer = bitmap[0].Create(hdc, 600, 600);
	//backBuffer = bitmap[0].Init(hdc, hInstance, blackBackStr.c_str());
	for(int i = 1; i <= 40; i++)
		bitmap[i].Init(hdc, hInstance, fileNames[i - 1].c_str());
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}
ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

void ResourceManager::Release()
{
	delete[] bitmap;
	delete mthis;
}