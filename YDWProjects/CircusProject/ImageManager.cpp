#include "ImageManager.h"

ImageManager* ImageManager::mthis = nullptr;

void ImageManager::Init(HDC hdc)
{
	
}

void ImageManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	TransparentBlt(hdc, x, y, cx, cy, memDC[imageNum], 0, 0, cx, cy, RGB(255, 0, 255));
	BitBlt(hdc, 0, 0, cx, cy, memDC[0], 0, 0, SRCCOPY);
}

ImageManager::ImageManager()
{
}


ImageManager::~ImageManager()
{
}

