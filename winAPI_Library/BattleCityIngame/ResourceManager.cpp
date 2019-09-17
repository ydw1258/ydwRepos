#include "ResourceManager.h"
#include<iostream>
using namespace std;

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	string fileNames[50] = {
							"BattleResource/__empty.bmp",
							"BattleResource/__brick.bmp",
							"BattleResource/__block.bmp",
							"BattleResource/__water.bmp",
							"BattleResource/__bush.bmp",
							"BattleResource/__steel.bmp",

							"BattleResource/_row-1-col-1.bmp",
							"BattleResource/_row-1-col-2.bmp",
							"BattleResource/_row-1-col-3.bmp",
							"BattleResource/_row-1-col-4.bmp",
							"BattleResource/_row-1-col-5.bmp",
							"BattleResource/_row-1-col-6.bmp",
							"BattleResource/_row-1-col-7.bmp",
							"BattleResource/_row-1-col-8.bmp",
							"BattleResource/__eagle.bmp",
							"BattleResource/__flag.bmp",
							"BattleResource/__shield.bmp",
							"BattleResource/__bullet.bmp",
							"BattleResource/__explosion_big.bmp",
							"BattleResource/__explosion_small.bmp"
	};

	bitmap = new BitMap[50];

	backBuffer = bitmap[0].Create(hdc, 600, 600);
	//backBuffer = bitmap[0].Init(hdc, hInstance, blackBackStr.c_str());

	for (int i = 0; i < 25; i++)
	{
		bitmap[i + 1].Init(hdc, hInstance, fileNames[i].c_str());
	}
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