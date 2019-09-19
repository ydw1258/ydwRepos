#pragma once
#include"GameObject.h"
#include"Map.h"
#include<iostream>
#include<vector>
#include "NPC.h"

using namespace std;

class Player : public GameObject
{
private:
	bool isReading = false;
public:
	int atk;
	Player();
	~Player();
	void Init(int _x, int _y, int _hp, int _mp, int _atk);
	char Input(char ch, Map* map, NpcClass* npc);
	void Reading(NpcClass npc);
};