#include "GameManager.h"
#include<Windows.h>
using namespace std;
GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };
	startPos.x = 5 * TileImageSizeX;
	startPos.y = 13 * TileImageSizeY;

	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	//player.Init(3); // playerSpeed
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);

	for (int i = 0; i < 6; i++)
		blocks[i].Init(16, 16, (IMAGENUM)(IMAGENUM_EMPTY + i));
	blocks[IMAGENUM_EGLE].Init(32, 32, IMAGENUM_EGLE);
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
			if (i == 12 && j == 5)
			{
				mapTile[i * TILE_HEIGHT_NUM + j] = BLOCKEGLE;
				continue;
			}
				
			mapTile[i * TILE_HEIGHT_NUM + j] = (BLOCKTYPE)mapValues[i * TILE_HEIGHT_NUM + j];
			cout << mapValues[i * TILE_HEIGHT_NUM + j] << endl;
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

	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			DrawTile(hdc, mapTile[i * TILE_HEIGHT_NUM + j], j, i);
		}
	}
	//blocks[3].Draw(hdc, 680, 10 + GameOffsetY);
	BitBlt(hdc, 0, 0, 600, 600, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
}
void GameManager::DrawTile(HDC hdc, BLOCKTYPE blockType, int x, int y)
{
	const int MINIBLOCKSIZE = 16;

	switch (blockType)
	{
	case EMPTY:
		blocks[0].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[0].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[0].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[0].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BLICK_RIGHT:
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BLICK_DOWN:
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BLICK_LEFT:
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BLICK_UP:
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		break;
	case BLICK_FULL:
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[1].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case IRON_RIGHT:
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
	case IRON_DOWN:
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case IRON_LEFT:
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case IRON_UP:
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		break;
	case IRON_FULL:
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[2].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case WATER:
		blocks[3].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[3].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[3].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[3].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BUSH:
		blocks[4].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[4].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[4].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[4].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case ICE:
		blocks[5].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
		blocks[5].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY);
		blocks[5].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		blocks[5].Draw(hdc, x * TileImageSizeX + GameOffsetX + MINIBLOCKSIZE, y * TileImageSizeY + GameOffsetY + MINIBLOCKSIZE);
		break;
	case BLOCKEGLE:
		blocks[IMAGENUM_EGLE].Draw(hdc, x * TileImageSizeX + GameOffsetX, y * TileImageSizeY + GameOffsetY);
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
	
	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			if (mapTile[TILE_HEIGHT_NUM*i + j] == BLICK_RIGHT ||
				mapTile[TILE_HEIGHT_NUM*i + j] == BLICK_DOWN ||
				mapTile[TILE_HEIGHT_NUM*i + j] == BLICK_LEFT ||
				mapTile[TILE_HEIGHT_NUM*i + j] == BLICK_UP ||
				mapTile[TILE_HEIGHT_NUM*i + j] == BLICK_FULL ||
				mapTile[TILE_HEIGHT_NUM*i + j] == IRON_RIGHT ||
				mapTile[TILE_HEIGHT_NUM*i + j] == IRON_DOWN ||
				mapTile[TILE_HEIGHT_NUM*i + j] == IRON_LEFT ||
				mapTile[TILE_HEIGHT_NUM*i + j] == IRON_UP ||
				mapTile[TILE_HEIGHT_NUM*i + j] == IRON_FULL ||
				mapTile[TILE_HEIGHT_NUM*i + j] == BLOCKEGLE ||
				mapTile[TILE_HEIGHT_NUM*i + j] == WATER)
			{
				RECT Rect;
				Rect.top = i * TileImageSizeY + GameOffsetY;
				Rect.bottom = (i + 1) * TileImageSizeY + GameOffsetY;
				Rect.left = j * TileImageSizeX + GameOffsetX;
				Rect.right = (j + 1) * TileImageSizeX + GameOffsetX;
			
				if (Physics::GetInstance()->RECTbyRECTCollisionCheck(playerRect, Rect))
				{
					RECT rcTemp;

					//IntersectRect(&rcTemp, &Rect, &playerRect);
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
	cout << player.y << endl;
	cout << player.direction << endl;

	return false;
}
void GameManager::CollisionDraw(Player player, HDC hdc)
{
	RECT playerRect;
	playerRect.top = player.y;
	playerRect.bottom = player.y + 32;
	playerRect.left = player.x;
	playerRect.right = player.x + 32;

	Rectangle(hdc, playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);

	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			if (mapTile[TILE_HEIGHT_NUM * i + j] == BLICK_RIGHT ||
				mapTile[TILE_HEIGHT_NUM * i + j] == BLICK_DOWN ||
				mapTile[TILE_HEIGHT_NUM * i + j] == BLICK_LEFT ||
				mapTile[TILE_HEIGHT_NUM * i + j] == BLICK_UP ||
				mapTile[TILE_HEIGHT_NUM * i + j] == BLICK_FULL ||
				mapTile[TILE_HEIGHT_NUM * i + j] == IRON_RIGHT ||
				mapTile[TILE_HEIGHT_NUM * i + j] == IRON_DOWN ||
				mapTile[TILE_HEIGHT_NUM * i + j] == IRON_LEFT ||
				mapTile[TILE_HEIGHT_NUM * i + j] == IRON_UP ||
				mapTile[TILE_HEIGHT_NUM * i + j] == IRON_FULL ||
				mapTile[TILE_HEIGHT_NUM * i + j] == WATER)
			{
				RECT Rect;
				Rect.top = i * TileImageSizeY + GameOffsetY;
				Rect.bottom = (i + 1) * TileImageSizeY + GameOffsetY;
				Rect.left = j * TileImageSizeX + GameOffsetX;
				Rect.right = (j + 1) * TileImageSizeX + GameOffsetX;

				Rectangle(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
			}
		}
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