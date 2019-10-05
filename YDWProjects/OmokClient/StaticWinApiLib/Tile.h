#pragma once
#include"BitMap.h"

class Tile
{
protected:
	BitMap* bitMap;
	BitMap* BackBitMap;
	int			x;
	int			y;
public:
	void Init(BitMap* pBitMap, BitMap* pBackBitMap, int x, int y);
	void Draw(HDC hdc);
	bool Input(POINT pt);

	Tile();
	~Tile();
};
