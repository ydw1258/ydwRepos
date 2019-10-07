#include "GameManager.h"
#include<Windows.h>
using namespace std;
GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };
	startPos.x = 5 * TileImageSizeX;
	startPos.y = 13 * TileImageSizeY;

	//player.Init(3); // playerSpeed
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);

	for (int i = 0; i < 6; i++)
		blocks[i].Init(16, 16, (IMAGENUM)(IMAGENUM_EMPTY + i));
	blocks[IMAGENUM_EGLE].Init(32, 32, IMAGENUM_EGLE);
	grayBackGround.Init(IMAGENUM_GRAY_BACKGROUND, 1, 1000, 1000, 0, 0);

	string fileName = "C:\\stage01.txt";
	string buffer;
	int mapValues[TILE_HEIGHT_NUM * TILE_WIDTH_NUM];

	ifstream inFile(fileName);

	int count = 0;

	while (inFile.peek() != EOF) {
		// std::getline은 입력 스트림에서 string으로 한 줄을 읽습니다.
		getline(inFile, buffer);
		mapValues[count++] = atoi(buffer.c_str());
	}
	GameManager::GetInstance()->LoadMap(mapValues);

	
}
void GameManager::LoadMap(int* mapValues)
{
	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{

			switch (mapValues[TILE_HEIGHT_NUM * i + j]) //1, 1 => 3, 3  3, 4,  4,3  4,4
			{
			case EMPTY:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_RIGHT:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case BLICK_DOWN:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case BLICK_LEFT:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_UP:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_FULL:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case IRON_RIGHT:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case IRON_DOWN:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case IRON_LEFT:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case IRON_UP:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case IRON_FULL:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case WATER:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_WATER;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_WATER;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_WATER;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_WATER;
				break;
			case BUSH:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BUSH;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BUSH;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BUSH;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BUSH;
				break;
			case ICE:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_STEEL;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_STEEL;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_STEEL;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_STEEL;
				break;
			case BLOCKEGLE:
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2) + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[SMALL_TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;

				break;
			}
			cout << endl;
		}
	}
}

void GameManager::Shot(Player player)
{
  	if (bullets.size() == 0)
	{
		Bullet bullet;
		bullet.Init(player.x, player.y, player.direction);
		bullets.push_back(bullet);
	}
}
void GameManager::MoveBullets()
{
	for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end();)
	{
		it->Move();
		if (it->OutofRange())
			it = bullets.erase(it);
		else
			it++;
	}
}
void GameManager::DrawBullets(HDC hdc)
{
	for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++)
	{
		it->Draw(hdc);
	}
}
//Draw 매프레임 호출
void GameManager::Draw(HDC hdc)
{
	ResourceManager::GetInstance()->Draw(hdc, 0, 0, 600, 600, IMAGENUM_BLACKBACKGROUND);
	grayBackGround.DrawObject(hdc, 0, 0);
	for (int i = 0; i < SMALL_TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < SMALL_TILE_WIDTH_NUM; j++)
		{
			DrawTile(hdc, judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j], j, i);
			//cout << judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] << ", ";
			//cout << i << ", " << j << endl;

		}
	}
	BitBlt(hdc, 0, 0, 600, 600, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
}
void GameManager::DrawTile(HDC hdc, JUDGEBLOCKTYPE blockType, int x, int y)
{
	const int MINIBLOCKSIZE = 16;

	switch (blockType)
	{
	case JUDGE_EMPTY:
		blocks[0].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	case JUDGE_BRICK:
		blocks[1].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	case JUDGE_BLOCK:
		blocks[2].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	case JUDGE_WATER:
		blocks[3].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	case JUDGE_BUSH:
		blocks[4].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	case JUDGE_STEEL:
		blocks[5].Draw(hdc, x * MINIBLOCKSIZE + GameOffsetX, y * MINIBLOCKSIZE + GameOffsetY);
		break;
	default:
		break;
	}
}

bool GameManager::CollisionCheck(Player& player)
{
	RECT playerRect;
	playerRect.top = player.y;
	playerRect.bottom = player.y + 32;
	playerRect.left = player.x;
	playerRect.right = player.x + 32;
	
	for (int i = 0; i < SMALL_TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < SMALL_TILE_WIDTH_NUM; j++)
		{

			if(judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BRICK ||
				judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BLOCK ||
				judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_WATER
				)
			{
				RECT Rect;
				Rect.top = i * TileImageSizeY / 2 + GameOffsetY;
				Rect.bottom = (i + 1) * TileImageSizeY / 2 + GameOffsetY;
				Rect.left = j * TileImageSizeX / 2+ GameOffsetX;
				Rect.right = (j + 1) * TileImageSizeX / 2 + GameOffsetX;
			
				if (Physics::GetInstance()->RECTbyRECTCollisionCheck(playerRect, Rect))
				{
					RECT rcTemp;

					if (playerRect.top > Rect.top && player.direction == UP) //player가 아래에 있을때
					{
						player.y++;
						return true;
					}
					else if(playerRect.top < Rect.top && player.direction == DOWN)
					{
						player.y--;
						return true;
					}
					if (playerRect.left > Rect.left && player.direction == LEFT) //player가 오른쪽에 있을때
					{
						player.x++;
						return true;
					}
					else if(playerRect.left < Rect.left && player.direction == RIGHT)
					{
						player.x--;
						return true;
					}
					return true;
				}
					
			}
		}
	}
	CollisionCheckBullet();
	return false;
}
void GameManager::CollisionCheckBullet()
{

	for (int i = 0; i < SMALL_TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < SMALL_TILE_WIDTH_NUM; j++)
		{
			if (judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BRICK)
			{
				for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end();)
				{
					RECT bulletRect;
					bulletRect.top = it->y - TileImageSizeY / 2;
					bulletRect.bottom = it->y + TileImageSizeY / 2;
					bulletRect.left = it->x - TileImageSizeY / 2;
					bulletRect.right = it->x + TileImageSizeY / 2;

					RECT Rect;
					Rect.top = i * TileImageSizeY / 2 + GameOffsetY;
					Rect.bottom = (i + 1) * TileImageSizeY / 2 + GameOffsetY;
					Rect.left = j * TileImageSizeX / 2 + GameOffsetX;
					Rect.right = (j + 1) * TileImageSizeX / 2 + GameOffsetX;

					if (Physics::GetInstance()->RECTbyRECTCollisionCheck(Rect, bulletRect))
					{
						it = bullets.erase(it);
						CollisionBlockandBullet(bulletRect, j, i);
					}
					else
					{
						it++;
					}
						
				}
			}
			else if (judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BLOCK)
			{
				for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end();)
				{
					RECT bulletRect;
					bulletRect.top = it->y - TileImageSizeY / 4;
					bulletRect.bottom = it->y + TileImageSizeY / 2;
					bulletRect.left = it->x - TileImageSizeX / 4;
					bulletRect.right = it->x + TileImageSizeX / 2;

					RECT Rect;
					Rect.top = i * TileImageSizeY / 2 + GameOffsetY;
					Rect.bottom = (i + 1) * TileImageSizeY / 2 + GameOffsetY;
					Rect.left = j * TileImageSizeX / 2 + GameOffsetX;
					Rect.right = (j + 1) * TileImageSizeX / 2 + GameOffsetX;

					if (Physics::GetInstance()->RECTbyRECTCollisionCheck(Rect, bulletRect))
					{
						it = bullets.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}
	}
}
void GameManager::CollisionBlockandBullet(RECT bulletRect, int colisionBlockX, int colisionBlockY)
{
	//총알의 위치, 부딪힌 벽돌의 위치관계
	judgeMapTile[colisionBlockY * SMALL_TILE_HEIGHT_NUM + colisionBlockX] = JUDGE_EMPTY;
}
void GameManager::CollisionDraw(Player player, HDC hdc)
{
	if (!isCollisionViewOn)
		return;
	RECT playerRect;
	playerRect.top = player.y;
	playerRect.bottom = player.y + 32;
	playerRect.left = player.x;
	playerRect.right = player.x + 32;

	Rectangle(hdc, playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);

	for (int i = 0; i < SMALL_TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < SMALL_TILE_WIDTH_NUM; j++)
		{
			if (judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BRICK ||
				judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_BLOCK ||
				judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j] == JUDGE_WATER )
			{
				RECT Rect;
				Rect.top = i * TileImageSizeY / 2 + GameOffsetY;
				Rect.bottom = (i + 1) * TileImageSizeY / 2 + GameOffsetY;
				Rect.left = j * TileImageSizeX / 2 + GameOffsetX;
				Rect.right = (j + 1) * TileImageSizeX / 2 + GameOffsetX;

				Rectangle(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
			}
		}
	}
	for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end();it++)
	{
		RECT bulletRect;
		bulletRect.top = it->y - TileImageSizeY / 4;
		bulletRect.bottom = it->y + TileImageSizeY / 2;
		bulletRect.left = it->x - TileImageSizeX / 4;
		bulletRect.right = it->x + TileImageSizeX / 2;

		Rectangle(hdc, bulletRect.left, bulletRect.top, bulletRect.right, bulletRect.bottom);
	}
}
void GameManager::DrawBlack()
{
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	myBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->backBuffer->GetmemDC(), myBrush);
	Rectangle(ResourceManager::GetInstance()->backBuffer->GetmemDC(), 0, 0, 600, 600);
	SelectObject(ResourceManager::GetInstance()->backBuffer->GetmemDC(), oldBrush);
	DeleteObject(myBrush);
}
GameManager::GameManager() {}
GameManager::~GameManager() {}