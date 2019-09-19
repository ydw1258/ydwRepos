#include<iostream>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>
#include<list>
using namespace std;

#define FPS 60.0f
#define WIDTH 19
#define HEIGHT 19
int map[HEIGHT][WIDTH];

#define DRAW_EMPTY 0
#define DRAW_WALL 1
#define ENEMYBULLET 2

int playerX = WIDTH / 2;
int playerY = HEIGHT - 1;
int score = 0;

struct Bullet
{
	int x;
	int y;
	int speed;
};

void Init()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == 0 || j == WIDTH - 1)
			{
				map[i][j] = DRAW_WALL;
			}
			else
				map[i][j] = DRAW_EMPTY;
		}
	}
}
void Draw()
{
	for(int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (playerX == j && playerY == i)
			{
				cout << "○";
			}
			else if (map[i][j] == DRAW_WALL)
			{
				cout << "☆";
			}
			else if (map[i][j] == ENEMYBULLET)
			{
				cout << "★";
			}
			else
				cout << "  ";
		}
		cout << endl;
	}
}
void Input()
{
	if (kbhit())
	{
		char ch = getch();

		if (ch == 'a')
		{
			if(map[playerY][playerX - 1] != DRAW_WALL)
				playerX--;
		}
		else if (ch == 'd')
		{
			if (map[playerY][playerX + 1] != DRAW_WALL)
			playerX++;
		}
	}
}
int MovingEnemyBullet()
{
	for (int i = HEIGHT - 1; i >= 0; i--)
	{
		for (int j = WIDTH - 1; j >= 0; j--)
		{
			if (map[i][j] == ENEMYBULLET)
			{
				if (playerX == j && playerY == i)
					return 1;
				if (i == HEIGHT - 1)
				{
					map[i][j] = DRAW_EMPTY;
					score++;
				}
				else
				{
					map[i][j] = DRAW_EMPTY;
					map[i + 1][j] = ENEMYBULLET;
				}
			}
		}
	}
	return 0;
}

int main()
{
	Init();
	Bullet bullet[20];

	while (true)
	{
		cout << "점수 : " << score << endl;
		int spawnbullet = rand() % 10;
		int spawnX = (rand() % (WIDTH - 2)) + 1;

		if (spawnbullet <= 9)
			map[0][spawnX] = ENEMYBULLET;

		Input();
		Draw();
		if (MovingEnemyBullet() == 1)
		{
			cout << "게임 종료";
			break;
		}
		
		Sleep(1000 / FPS);
		system("cls");
	}

	return 0;
}