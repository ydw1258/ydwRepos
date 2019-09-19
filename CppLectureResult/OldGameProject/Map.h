#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include"GameObject.h"

#define DRAW_EMPTY '0'
#define	DRAW_WALL '1'
#define DRAW_PLAYER '2'
#define DRAW_ITEM '3'
#define DRAW_MONSTER '4'
#define DRAW_NPC '5'
#define DRAW_PORTAL '6'

using namespace std;

class Map
{
private:
	int LoadedMapIndex;
public:
	vector<string> map;
	Map();
	~Map();
	void LoadMap(int& index);
	void DrawMap(vector<GameObject*>* gameObject, int size);
};

