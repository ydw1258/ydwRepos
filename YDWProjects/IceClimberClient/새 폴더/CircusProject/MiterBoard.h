#pragma once
#include "FontManager.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

class MiterBoard
{
	SpriteRenderer Image;
	FontManager _font;
public:
	int x;
	int y;
	void Init();
	void Draw(int distance, int x, int y, int scrollSpeedX = 0, int scrollSpeedY = 0);
	MiterBoard();
	~MiterBoard();
};