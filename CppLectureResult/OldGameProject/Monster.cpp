#include "Monster.h"

Monster::Monster()
{
	objectType = MONSTER;
}

Monster::~Monster()
{
}

void Monster::Init(int _x, int _y, int _hp, int _mp, int _atk)
{
	x = _x;
	y = _y;
	hp = _hp;
	mp = _mp;
	atk = _atk;
}
