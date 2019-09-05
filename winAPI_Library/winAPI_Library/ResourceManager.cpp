#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	
	bitmap = new BitMap[30];
	/*
	backBuffer = bitmap[0].Create(hdc, 1024, 720);
	bitmap[1].Init(hdc, hInstance, "back.bmp");
	bitmap[2].Init(hdc, hInstance, "back_deco.bmp");
	*/
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}
ResourceManager::ResourceManager(){}
ResourceManager::~ResourceManager(){}

void ResourceManager::Release()
{
	delete[] bitmap;
	delete mthis;
}