#pragma once
#include"BitMap.h"
#include "SpriteRenderer.h"

class Tile
{
protected:
	SpriteRenderer sprite;
	int	sizeX;
	int	sizeY;
public:
	void Init(int _sizeX, int _sizeY, IMAGENUM imageNum);
	void Draw(HDC hdc, int x, int y);

	Tile();
	~Tile();
};
