#pragma once
#include"GameObject.h"

class Portal : public GameObject
{
	int moveMapIndex;
public:
	int movingPlayerX;
	int movingPlayerY;
	Portal();
	~Portal();
	void Init(int _x, int _y, int _moveMapIndex, int movingPlayerX, int movingPlayerY);
};