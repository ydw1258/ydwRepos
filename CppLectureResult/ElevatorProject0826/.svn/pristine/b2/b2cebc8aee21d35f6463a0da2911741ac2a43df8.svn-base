#include<iostream>
#include<conio.h>
using namespace std;

#define WIDTH 19
#define HEIGHT 19

#define EMPTY 0
#define WALL 1
int map[HEIGHT][WIDTH];

struct Player
{
	int x;
	int y;
};
void Init()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
			{
				map[i][j] = WALL;
			}
			else
				map[i][j] = EMPTY;
		}
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

		}
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

		}
	}
}
void Drawmap(Player player)
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (j == player.x && i == player.y)
				cout << "¡Û";
			else if(map[i][j] == EMPTY)
			{
				cout << "¡Ù";
			}
		}
		cout << endl;
	}
}
void playerControl(Player* player)
{
	char input = getch();

	if (input == 'w')
	{
		player->y--;
	}
	else if (input == 'a')
	{
		player->x--;
	}
	else if (input == 's')
	{
		player->y++;
	}
	else if (input == 'd')
	{
		player->x++;
	}
}
int main()
{
	//Init
	Player player = { WIDTH / 2, HEIGHT / 2 };

	int turn = 0;
	int map[HEIGHT][WIDTH] = { };
	
	while (true)
	{
		Drawmap(player);
		playerControl(&player);

		system("cls");
	}

	return 0;
}