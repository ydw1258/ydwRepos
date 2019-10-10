#include "ResourceManager.h"

#include<iostream>
using namespace std;

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC _hdc, string* fileNames, int fileNameCount)
{
	hdc = _hdc;

	bitmap = new BitMap[50];
	backBuffer = &bitmap[0];
	backBuffer->Init(_hdc, 1000, 1000);

	for (int i = 0; i < fileNameCount; i++)
	{
		bitmap[i + 1].Init(hdc, fileNames[i].c_str()); //hInstance가 필요하답니다.
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