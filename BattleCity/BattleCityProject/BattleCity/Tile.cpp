#include "Tile.h"

Tile::Tile(){}
Tile::~Tile(){}

void Tile::Init(int _sizeX, int _sizeY, IMAGENUM imageNum)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	sprite.Init(imageNum, 1, sizeX, sizeY);
}

void Tile::Draw(HDC hdc, int x, int y)
{
	sprite.DrawObject(hdc, x, y);
}

