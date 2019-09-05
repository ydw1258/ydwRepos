#include "Tile.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::Init(BitMap* pBitMap, BitMap* pBackBitMap, int x, int y)
{
	bitMap = pBitMap;
	BackBitMap = pBackBitMap;
	x = x;
	y = y;
}

void Tile::Draw(HDC hdc)
{
	bitMap->Draw(hdc, x, y, 20, 20);
}

bool Tile::Input(POINT pt)
{
	RECT rc = { x , y , x + 20  , y + 20 };

	if (PtInRect(&rc, pt))
	{
		//클릭한 위치

	}

	return false;
}
