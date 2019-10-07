#include "Goal.h"

void Goal::Init(int _x, int _y)
{
	x = _x;
	y = _y;
	sprite.Init(IMAGENUM_END, 1, 76, 49, x, y);
}

void Goal::Draw(HDC hdc)
{
	sprite.DrawObject(hdc, x, y);
}

Goal::Goal(){ }
Goal::~Goal(){ }