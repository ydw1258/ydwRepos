#include "Map.h"

Map::Map() { }

void Map::Init()
{
	snakeDirection = DIR_RIGHT;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
				map[i][j] = WALL;
			else if (i == HEIGHT / 2 && j == WIDTH / 2)
				map[i][j] = SNAKEHEAD;
			else
				map[i][j] = EMPTY;
		}
	}
	FoodSpawn();
}
void Map::DrawMap()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch (map[i][j])
			{
			case WALL:
				cout << "□";
				break;
			case SNAKEBODY:
			case SNAKEHEAD:
			case SNAKETAIL:
				cout << "■";
				break;
			case FOOD:
				cout << "♥";
				break;
			case EMPTY:
				cout << "  ";
				break;
			}
		}
		cout << endl;
	}
}
void Map::SnakeMove(int length)
{
	//꼬리는 제거하고 머리를 방향에 따라 옮기기
	//먹이를 먹었으면 꼬리는 그대로
	bool flag = false;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			//head를 찾으면 break로 탈출12
			if (map[i][j] == SNAKEHEAD && length > 1)
			{
				flag = true;
				switch (snakeDirection)
				{
				case DIR_UP:
					map[i][j] = SNAKEBODY;
					map[i - 1][j] = SNAKEHEAD;
					break;
				case DIR_DOWN:
					map[i][j] = SNAKEBODY;
					map[i + 1][j] = SNAKEHEAD;
					break;
				case DIR_LEFT:
					map[i][j] = SNAKEBODY;
					map[i][j - 1] = SNAKEHEAD;
					break;
				case DIR_RIGHT:
					map[i][j] = SNAKEBODY;
					map[i][j + 1] = SNAKEHEAD;
					break;
				default:
					break;
				}
			}
			else if (map[i][j] == SNAKEHEAD && length == 1)
			{
				flag = true;
				switch (snakeDirection)
				{
				case DIR_UP:
					map[i][j] = EMPTY;
					map[i - 1][j] = SNAKEHEAD;
					break;
				case DIR_DOWN:
					map[i][j] = EMPTY;
					map[i + 1][j] = SNAKEHEAD;
					break;
				case DIR_LEFT:
					map[i][j] = EMPTY;
					map[i][j - 1] = SNAKEHEAD;
					break;
				case DIR_RIGHT:
					map[i][j] = EMPTY;
					map[i][j + 1] = SNAKEHEAD;
					break;
				default:
					break;
				}
			}
			if (flag)
				break;
		}
		if (flag)
			break;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			//head를 찾으면 break로 탈출
			if (map[i][j] == SNAKETAIL)
			{
				flag = true;
				switch (snakeDirection)
				{
				case DIR_UP:
					map[i][j] = EMPTY;
					map[i + 1][j] = SNAKETAIL;
					break;
				case DIR_DOWN:
					map[i][j] = SNAKETAIL;
					map[i - 1][j] = EMPTY;
					break;
				case DIR_LEFT:
					map[i][j] = EMPTY;
					map[i][j - 1] = SNAKETAIL;
					break;
				case DIR_RIGHT:
					map[i][j] = EMPTY;
					map[i][j + 1] = SNAKETAIL;
					break;
				default:
					break;
				}
			}
			if (flag)
				break;
		}
		if (flag)
			break;
	}
}
void Map::FoodSpawn()
{
	srand((unsigned)time(NULL));
	int x = rand() % WIDTH;
	int y = rand() % HEIGHT;
	
	map[x][y] = FOOD;
}

void Map::ChangeSnakeDirection(int key)
{
	switch (key)
	{
	case 72 :// 위
		snakeDirection = DIR_UP;
		break;
	case 75: //왼쪽
		snakeDirection = DIR_LEFT;
		break;
	case 77://오른쪽
		snakeDirection = DIR_RIGHT;
		break;
	case 80://아래
		snakeDirection = DIR_DOWN;
		break;
	}
}
bool Map::CrashCheck(int HeadX, int HeadY)
{
	switch (snakeDirection)
	{
	case DIR_UP:
		if (map[HeadY - 1][HeadX] == WALL || map[HeadY - 1][HeadX] == SNAKEBODY || map[HeadY - 1][HeadX] == SNAKETAIL)
			return true;
		else if (map[HeadY][HeadX - 1] == FOOD)
		{

		}
		break;
	case DIR_DOWN:
		if (map[HeadY + 1][HeadX] == WALL || map[HeadY + 1][HeadX] == SNAKEBODY || map[HeadY + 1][HeadX] == SNAKETAIL)
			return true;
		else if (map[HeadY + 1][HeadX] == FOOD)
		{

		}
		break;
	case DIR_LEFT:
		if (map[HeadY][HeadX - 1] == WALL || map[HeadY][HeadX - 1] == SNAKEBODY || map[HeadY][HeadX - 1] == SNAKETAIL)
			return true;
		else if (map[HeadY][HeadX - 1] == FOOD)
		{ 

		}
		break;
	case DIR_RIGHT:
		if (map[HeadY][HeadX + 1] == WALL || map[HeadY][HeadX + 1] == SNAKEBODY || map[HeadY][HeadX + 1] == SNAKETAIL)
			return true;
		else if (map[HeadY][HeadX + 1] == FOOD)
		{

		}
		break;
	default:
		break;
	}
	
	return false;
}
Map::~Map()
{
}