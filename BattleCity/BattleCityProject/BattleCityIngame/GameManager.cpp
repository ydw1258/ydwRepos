#include "GameManager.h"
#include<Windows.h>
using namespace std;

GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC _hdc, HINSTANCE _g_hInst, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[50] = { "BattleResource\\__empty.bmp",
							"BattleResource\\__brick.bmp",
							"BattleResource\\__block.bmp",
						"BattleResource\\__water.bmp",
						"BattleResource\\__bush.bmp",
						"BattleResource\\__steel.bmp",
		"BattleResource\\enemy_up01.bmp",
		"BattleResource\\enemy_up02.bmp",
		"BattleResource\\enemy_down01.bmp",
		"BattleResource\\enemy_down02.bmp",
		"BattleResource\\enemy_left01.bmp",//10 인덱스
		"BattleResource\\enemy_left02.bmp",
		"BattleResource\\enemy_right01.bmp",
		"BattleResource\\enemy_right02.bmp",
		"BattleResource\\player_up01.bmp",
		"BattleResource\\player_up02.bmp",
		"BattleResource\\player_down01.bmp",
		"BattleResource\\player_down02.bmp",
		"BattleResource\\player_left01.bmp",
		"BattleResource\\player_left02.bmp",
		"BattleResource\\player_right01.bmp",
		"BattleResource\\player_right02.bmp",
		"BattleResource\\flash01.bmp",
		"BattleResource\\flash02.bmp",
		"BattleResource\\flash03.bmp",
		"BattleResource\\flash04.bmp", //25
		"BattleResource\\enemyIcon.bmp",
		"BattleResource\\logo.bmp", //27
		"BattleResource\\__eagle.bmp",
		"BattleResource\\__crashedeagle.bmp",
		"BattleResource\\bullet.bmp"
	};
	player.Init();
	ResourceManager::GetInstance()->Init(_hdc, filename, 30);
	FontManager::Init();
	ResourcesInit();
	LoadMap();
	backBlackImage.Init(_hdc, 600, 600, 0, 0, 0);
}

void GameManager::Update(float deltaTime)
{
	cout << greyScreen[0].bottom << endl;
	
	switch (scene)
	{
	case TITLE:
		ScreenScroll(deltaTime);
		break;
	case INGAME:
		//SpriteChance필요

		ScreenScroll(deltaTime);
		//MoveBullets();
		break;
	default:
		break;
	}
}
void GameManager::Draw(HDC hdc)
{
	switch (scene)
	{
	case TITLE:
		DrawBlack(hdc);
		DrawTitle(hdc);
		DrawScreen(hdc);
		break;
	case INGAME:
		DrawGrey(hdc);
		DrawBlack(hdc);
		DrawIngame(hdc);
		DrawScreen(hdc);
		break;
	case RESULT:
		break;
	}
	DrawGameInfo(hdc);
}

void GameManager::LoadMap()
{
	int mapValues[TILE_HEIGHT_NUM * TILE_WIDTH_NUM];

	int count = 0;
	ifstream openFile("BattleResource\\stage01.txt");

	if (openFile.is_open())
	{
		string line;
		while (getline(openFile, line))
		{
			//split
			char* tok2 = strtok((char *)line.c_str(), "/");
			while (tok2 != NULL)
			{
				mapTile[count++] = (BLOCKTYPE)atoi(tok2);
				tok2 = strtok(NULL, "/");
			}
		}
	}
	openFile.close();

	/*for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			if (i == 12 && j == 5)
			{
				mapTile[i * TILE_HEIGHT_NUM + j] = BLOCKEGLE;
				continue;
			}
				
			mapTile[i * TILE_HEIGHT_NUM + j] = (BLOCKTYPE)mapValues[i * TILE_HEIGHT_NUM + j];
		}
	}*/
	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			switch (mapTile[TILE_HEIGHT_NUM * i + j]) 
			{
			case EMPTY://
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_RIGHT:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case BLICK_DOWN:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case BLICK_LEFT:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_UP:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case BLICK_FULL:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BRICK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BRICK;
				break;
			case IRON_RIGHT:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case IRON_DOWN:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case IRON_LEFT:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case IRON_UP:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			case IRON_FULL:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BLOCK;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BLOCK;
				break;
			case WATER:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_WATER;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_WATER;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_WATER;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_WATER;
				break;
			case BUSH:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_BUSH;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_BUSH;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_BUSH;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_BUSH;
				break;
			case ICE:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_STEEL;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_STEEL;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_STEEL;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_STEEL;
				break;
			case BLOCKEGLE:
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * i * 2 + j * 2 + 1] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2] = JUDGE_EMPTY;
				judgeMapTile[TILE_HEIGHT_NUM * (i * 2 + 1) + j * 2 + 1] = JUDGE_EMPTY;
				break;
			}
		}
	}
}

void GameManager::PlayerMove(DIRECTION dir, float deltaTime)
{
	player.PlayerMove(dir, deltaTime);
}

void GameManager::Shot()
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

void GameManager::ResourcesInit()
{
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };
	startPos.x = 5 * TileImageSizeX;
	startPos.y = 13 * TileImageSizeY;

	for (int i = 0; i < 11; i++)
		blockSprites[i].Init((IMAGENUM)(IMAGENUM_BLOCK + i), 1, 16, 16);
	TitleLogo.Init(IMAGENUM_LOGO, 1, 500, 151);
	//blocks[IMAGENUM_EGLE].Init(IMAGENUM_EGLE, 1, 32, 32); //손봐야함
		for (int i = 0; i < 10; i++)
			Timer[i] = 0;
	for (int i = 0; i < 6; i++)
		blocks[i].Init((IMAGENUM_EMPTY + i), 1, 16, 16);
	greyScreen[0] = { 0, 0, 600, 0 };
	greyScreen[1] = { 0, 600, 600, 600 };

	flash.Init(IMAGENUM_FLASH_01, 4, 16, 16);
}

void GameManager::DrawTitle(HDC hdc)
{
	TitleLogo.DrawObject(hdc, 30, logoOffsetY);
	FontManager font;

	if(!isScreenScroll)
		font.Draw(hdc, "게임 시작 space", 30, 150, 400, "DungGeunMo", RGB(128, 128, 128));
	
}

void GameManager::DrawIngame(HDC hdc)
{
	//DrawTile을 그리는거 찾기
	player.Draw(hdc);
	//DrawScreen(hdc);
	//DrawBullets(hdc);
}
void GameManager::DrawScreen(HDC hdc)
{
	HBRUSH tempBrush, tempOBrush;
	tempBrush = CreateSolidBrush(RGB(192, 192, 192));
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
	Rectangle(hdc, greyScreen[0].left, greyScreen[0].top, greyScreen[0].right, greyScreen[0].bottom);
	Rectangle(hdc, greyScreen[1].left, greyScreen[1].top, greyScreen[1].right, greyScreen[1].bottom);
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);
}
void GameManager::DrawTile(HDC hdc)
{
	for (int i = 0; i < SMALL_TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < SMALL_TILE_WIDTH_NUM; j++)
		{
			switch (judgeMapTile[i * SMALL_TILE_HEIGHT_NUM + j])
			{
			case JUDGE_EMPTY:
				blocks[0].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;
			case JUDGE_BRICK:
				blocks[1].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;					  
			case JUDGE_BLOCK:			  
				blocks[2].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;					  
			case JUDGE_WATER:			  
				blocks[3].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;					  
			case JUDGE_BUSH:			  
				blocks[4].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;					  
			case JUDGE_STEEL:			  
				blocks[5].DrawObject(hdc, j * MINIBLOCK_SIZE + GameOffsetX, i * MINIBLOCK_SIZE + GameOffsetY);
				break;
			default:
				break;
			}
		}
	}
	
}

void GameManager::DrawGameInfo(HDC hdc)
{
	FontManager font;
	font.Draw(hdc, "프레임", 20, 500, 0, "DungGeunMo", RGB(255, 255, 255));
	switch (scene)
	{
	case TITLE:
		break;
	case INGAME:
		//font.Draw(hdc, (int)player.x, 20, 500, 30, "DungGeunMo", RGB(255, 255, 255));
		//font.Draw(hdc, (int)player.y, 20, 500, 50, "DungGeunMo", RGB(255, 255, 255));
		break;
	}
}

bool GameManager::CollisionCheck()
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

	return false;
}
void GameManager::CollisionDraw(HDC hdc)
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

void GameManager::DrawBlack(HDC hdc)
{
	HBRUSH tempBrush, tempOBrush;
	tempBrush = CreateSolidBrush(RGB(0, 0, 0));
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
	Rectangle(hdc, 20, 20, 620, 620);
	//tempOBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);

}
void GameManager::DrawGrey(HDC hdc)
{
	HBRUSH tempBrush, tempOBrush;
	tempBrush = CreateSolidBrush(RGB(192, 192, 192));
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
	Rectangle(hdc, 0, 0, 800, 800);
	//tempOBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);
}
void GameManager::ScreenScroll(float deltaTime)
{
	static bool screenSwitch = false;
	static float timer = 0;
	static bool flag =false;

	if (isScreenScroll)
	{
		logoOffsetY -= 2;
		if (logoOffsetY < 100)
		{
			isScreenScroll = false;
		}
	}
	int scrollSpeed = deltaTime * 500;

	if (isScreenMoving) 
	{
		if (screenSwitch) //여는 중
		{
			timer += deltaTime;
			
			if (timer > 1) //닫혀있는 중
			{
				greyScreen[0].bottom -= scrollSpeed;
				greyScreen[1].top += scrollSpeed;
			}
			else//다시 열기
			{
				if (!flag)
				{
					//열리는 순간 맵로드, 씬변경
					flag = true;
					scene = INGAME;
				}
				/*HBRUSH tempBrush, tempOBrush;
				tempBrush = CreateSolidBrush(RGB(192, 192, 192));
				tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
				Rectangle(hdc, 0, 0, 600, 600);
				tempOBrush = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, tempOBrush);
				DeleteObject(tempBrush);*/

				//FontManager font;
				//font.Draw(hdc, "스테이지 시작", 30, 200, 300, "DungGeunMo", RGB(0, 0, 0));
			}
		}
		else //닫는 중
		{
			greyScreen[0].bottom += scrollSpeed;
			greyScreen[1].top -= scrollSpeed;
		}

		if (greyScreen[0].bottom >= 300)
		{
			screenSwitch = true;
		}
		if (greyScreen[0].bottom < 0)
		{
			isScreenMoving = false;
			screenSwitch = false;
		}
	}
}

//현재 무의미
void GameManager::SceneChange(Scene _scene, float delayTime)
{
	if (isScreenScroll)
	{
		logoOffsetY= 100;

		isScreenScroll = false;
		return;
	}
	switch (scene)
	{
	case TITLE:
		isScreenMoving = true;
		//스크린 닫혔다 열기
		break;
	case INGAME:
		//
		break;
	case RESULT:
		break;
	default:
		break;
	}
}
void GameManager::Release()
{
	bullets.clear();

	ResourceManager::GetInstance()->Release();
	Physics::GetInstance()->Release();
	delete this;
}
GameManager::GameManager() {}
GameManager::~GameManager() {}