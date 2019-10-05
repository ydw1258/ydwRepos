#include<iostream>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>
#include<list>
#include"Player.h"
#include"Crater.h"
#include"Map.h"
using namespace std;

#define FPS 60.0f

int main()
{
	Map map;
	Player player;
	//map.Init();
	player.Init(&map);

	Crater crater[20];

	for (int i = 0; i < 20; i++)
	{
		crater[i].setPosition(rand() % (WIDTH - 2), 2, 3);
	}

	while (true)
	{
		cout << "Á¡¼ö : " << player.score << endl;
		if (player.OperateInput())
			break; 
		
		map.DrawMap();
	}

	return 0;
}