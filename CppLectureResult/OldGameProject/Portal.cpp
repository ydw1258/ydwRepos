#include "Portal.h"

Portal::Portal()
{
	objectType = PORTAL;
}


Portal::~Portal()
{
}

void Portal::Init(int _x, int _y, int _moveMapIndex, int _movingPlayerX, int _movingPlayerY)
{
	x = _x;
	y = _y;
	moveMapIndex = _moveMapIndex;
	movingPlayerX = _movingPlayerX;
	movingPlayerY = _movingPlayerY;
}