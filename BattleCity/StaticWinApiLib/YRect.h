#pragma once
#include <Windows.h>
#include "ResourceManager.h"

class YRect
{
	RECT myRect;
public:
	YRect(RECT* rect);
	YRect(RECT& rect);
	~YRect();
	void DrawRect(HDC hdc, int x, int y, int cx, int cy, int imageNum, RGBInfo rgb = { 0 });
	void DrawTransparentRect(HDC hdc, int x, int y, int cx, int cy, int imageNum, RGBInfo rgb = { 0 });
};