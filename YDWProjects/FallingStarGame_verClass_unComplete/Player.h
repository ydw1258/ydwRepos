#pragma once
#include"Map.h"

class Player
{
private:
	int x;
	int y;
	Map* pMap;
public:
	
	int score;

	Player();
	
	~Player();
	void Init(Map* pMap);
	bool OperateInput();
};