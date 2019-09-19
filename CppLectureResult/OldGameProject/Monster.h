#pragma once
#include"GameObject.h"

class Monster : public GameObject 
{
	bool isBoss = false;
public:
	int atk;
	void Init(int _x, int _y, int _hp, int _mp, int _atk);
	Monster();
	~Monster();
};

