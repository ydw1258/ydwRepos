#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	string fileNames[50] = {
							"BattleResource/__empty.bmp"
							"BattleResource/__brick.bmp",
							"BattleResource/__block.bmp",
							"BattleResource/__water.bmp",
							"BattleResource/__bush.bmp",
							"BattleResource/__steel.bmp",
	};

	bitmap = new BitMap[50];

	backBuffer = bitmap[0].Create(hdc, 600, 600);
	//backBuffer = bitmap[0].Init(hdc, hInstance, blackBackStr.c_str());
	
	bitmap[0].Init(hdc, hInstance, "BattleResource/__empty.bmp" );
	bitmap[1].Init(hdc, hInstance, "BattleResource/__brick.bmp");
	bitmap[2].Init(hdc, hInstance, "BattleResource/__block.bmp");
	bitmap[3].Init(hdc, hInstance, "BattleResource/__water.bmp");
	bitmap[4].Init(hdc, hInstance, "BattleResource/__bush.bmp");
	bitmap[5].Init(hdc, hInstance, "BattleResource/__steel.bmp");

	bitmap[6].Init(hdc, hInstance, "BattleResource/battlecityBack.bmp");
	/*for(int i = 1; i <= 6; i++)  "BattleResource/__steel.bmp",
		bitmap[i].Init(hdc, hInstance, fileNames[i -1].c_str());*/
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