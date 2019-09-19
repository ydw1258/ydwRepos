#include "ResourceManager.h"
#include<iostream>
using namespace std;

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance, string* fileNames)
{
	//"BattleResource/__empty.bmp",
	bitmap = new BitMap[50];

	backBuffer = bitmap[0].Create(hdc, 600, 600);
	//backBuffer = bitmap[0].Init(hdc, hInstance, blackBackStr.c_str());

	for (int i = 0; i < fileNames->size(); i++)
	{
		bitmap[i + 1].Init(hdc, hInstance, fileNames[i].c_str());
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