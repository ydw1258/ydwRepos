#pragma once
#include<iostream>
#include<ctime>
using namespace std;

#define HEIGHT 20
#define WIDTH 20

enum INSIDESTATE
{
	EMPTY,
	WALL,
	SNAKEHEAD,
	SNAKEBODY,
	SNAKETAIL,
	FOOD,
};
enum SNAKEDIRECTION
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};
class Map
{
private:
	SNAKEDIRECTION snakeDirection;
	int map[HEIGHT][WIDTH];
public:
	void Init();
	void FoodSpawn();
	bool CrashCheck(int HeadX, int HeadY);

	void DrawMap();
	void SnakeMove(int length);
	void ChangeSnakeDirection(int key);
	Map();
	~Map();
};

