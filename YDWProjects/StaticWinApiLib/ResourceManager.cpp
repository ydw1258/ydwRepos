#include "ResourceManager.h"

#include<iostream>
using namespace std;

ResourceManager* ResourceManager::mthis = nullptr;
//BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, string* fileNames, int fileNameCount)
{
	//"BattleResource/__empty.bmp",
	
	bitmap = new BitMap[50];

	//backBuffer = bitmap[0].Create(hdc, 1200, 1000);
	
	
	for (int i = 0; i < fileNameCount; i++)
	{
		bitmap[i + 1].Init(hdc, fileNames[i].c_str());
	}
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, int imageNum)
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