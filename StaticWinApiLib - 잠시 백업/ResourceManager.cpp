#include "ResourceManager.h"

#include<iostream>
using namespace std;

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Release()
{
	for (int i = 0; i < 50; i++)
	{
		bitmap[i].Release();
	}
	delete[] bitmap;
	delete mthis;
}

void ResourceManager::Init(HDC _hdc, string* fileNames, int fileNameCount)
{
	bitmap = new BitMap[50];
	backBuffer = &bitmap[0];
	backBuffer->Init(_hdc, 1000, 1000);

	for (int i = 0; i < fileNameCount; i++)
	{
		bitmap[i + 1].Init(_hdc, fileNames[i].c_str()); //hInstance�� �ʿ��ϴ�ϴ�.
	}
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, int imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}
ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {
	
}
