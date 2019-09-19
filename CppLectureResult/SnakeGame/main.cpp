#include<iostream>
#include<ctime>
#include<windows.h>
#include"Map.h"
#include"Snake.h"
#include<list>
#include<vector>
#include<conio.h>

#define FPS 10
using namespace std;

class Position
{
	int x;
	int y;
};

int main()
{
	Map map;
	map.Init();
	vector<Position> snake;
	
	while (true)
	{
		map.DrawMap();
		map.SnakeMove(1);

		if (kbhit())
			map.ChangeSnakeDirection(getch());

		Sleep(1000 / FPS);
		system("cls");
	}
}