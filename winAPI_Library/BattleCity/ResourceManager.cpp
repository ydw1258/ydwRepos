#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	string fileNames[50] = {"BattleResource/__block.bmp",
							"BattleResource/__brick.bmp",
							"BattleResource/__bush.bmp",
							"BattleResource/__empty.bmp",
							"BattleResource/__steel.bmp",
							"BattleResource/_block.bmp",
							"BattleResource/_brick.bmp",
							"BattleResource/_bush.bmp",
							"BattleResource/_empty.bmp",
							"BattleResource/_steel.bmp",
							"BattleResource/_water.bmp",
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