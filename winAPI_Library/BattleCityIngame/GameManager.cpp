#include "GameManager.h"

using namespace std;
GameManager* GameManager::mthis = nullptr;


void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };

	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	//player.Init(3); // playerSpeed
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);

	for (int i = 0; i < 6; i++)
		blocks[i].Init(16, 16, (IMAGENUM)(IMAGENUM_EMPTY + i));

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
			mapTile[i * TILE_HEIGHT_NUM + j] = (BLOCKTYPE)mapValues[i * TILE_HEIGHT_NUM + j];
			cout << mapValues[i * TILE_HEIGHT_NUM + j] << endl;
		}
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
	default:
		break;
	}
}

bool GameManager::CollisionCheck(Player player)
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
				mapTile[TILE_HEIGHT_NUM*i + j] == WATER)
			{
				RECT Rect;
				Rect.top = i * TileImageSizeY;
				Rect.bottom = (i + 1) * TileImageSizeY;
				Rect.left = j * TileImageSizeX;
				Rect.right = (j + 1) * TileImageSizeX;
				
				if (Physics::GetInstance()->RECTbyRECTCollisionCheck(playerRect, Rect))
					return true;
			}
		}
	}

	return false;
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
