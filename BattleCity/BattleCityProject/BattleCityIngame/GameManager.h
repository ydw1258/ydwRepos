#pragma once
#include<Windows.h>
#include<string>
#include<list>
#include<vector>
#include<stack>
#include<iostream>
#include<fstream>
#include"SpriteRenderer.h"
#include"Tile.h"
#include"FontManager.h"
#include "Physics.h"
#include "Player.h"
#include "Bullet.h"
#include "IMAGENUM.h"
#include "EnemyManager.h"

using namespace std;

enum BLOCKTYPE
{
	EMPTY,
	BLICK_RIGHT,
	BLICK_DOWN,
	BLICK_LEFT,
	BLICK_UP,
	BLICK_FULL,
	IRON_RIGHT,
	IRON_DOWN,
	IRON_LEFT,
	IRON_UP,
	IRON_FULL,
	WATER,
	BUSH,
	ICE,
	BLOCKEGLE
};
enum JUDGEBLOCKTYPE // 1/4 짜리 타일
{
	JUDGE_EMPTY,
	JUDGE_BRICK,
	JUDGE_BLOCK,
	JUDGE_WATER,
	JUDGE_BUSH,
	JUDGE_STEEL
};

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TILE_WIDTH_NUM 13
#define TILE_HEIGHT_NUM 13

#define SMALL_TILE_WIDTH_NUM 26
#define SMALL_TILE_HEIGHT_NUM 26

#define MINIBLOCK_SIZE 16
#define TILEBLOCK_SIZE 32

using namespace std;

enum Scene
{
	TITLE,
	INGAME,
	RESULT
};
struct MapStackInfo
{
	POINT pt;
	BLOCKTYPE mapValue;
};

class GameManager
{
private:
	static GameManager* mthis;

	POINT playerStartPos;
	int score = 0;
	int remains;//남은 플레이어 기회
	HWND hwnd;
	//SpriteRenderer backGround;
	BitMap backBlackImage;
	SpriteRenderer blocks[15];
	SpriteRenderer TitleLogo;
	SpriteRenderer flash;
	SpriteRenderer blockSprites[11];

	//위아래로 닫혔다 열리는 회색 스크린
	RECT greyScreen[2]; 
	bool isScreenScroll = true;

	float logoOffsetY = 600.0f;
	
	EnemyManager enemyManager;
	bool GameOverflag = false;
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY }; //메모장에서 불러올때 사용하는 타일
	JUDGEBLOCKTYPE judgeMapTile[SMALL_TILE_HEIGHT_NUM * SMALL_TILE_WIDTH_NUM]; //1/4등분으로 나눈 판정 타일

	//list<Bullet> enemyBullets;
	list<Bullet> playerBullets;
public:
	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	Player player;
	Scene scene = TITLE;

	const int GameOffsetX = 20;
	const int GameOffsetY = 20;
	bool isGameOver = false;
	bool isScreenMoving = false;
	bool isCollisionView = false;
	
	
	void Draw(HDC hdc);
	void DrawTitle(HDC hdc);
	void DrawIngame(HDC hdc);
	void DrawGrey(HDC hdc);
	void DrawScreen(HDC hdc);
	void DrawBlack(HDC hdc);
	void DrawTile(HDC hdc);
	void DrawGameInfo(HDC hdc);
	bool CollisionCheck(float &x, float &y, DIRECTION direction);
	bool CollisionCheckBlockandBullet(float bulletX, float bulletY, DIRECTION direction);
	void CollisionDraw(HDC hdc);

	void Init(HDC hdc, HINSTANCE _g_hInst, HWND _hwnd);

	void Shot();
	void LoadMap();
	void PlayerMove(DIRECTION dir, float deltaTime);
	void PlayerBulletCollisionCheck();
	void MoveBullets(float deltaTime);
	void DrawBullets(HDC hdc);
	void ResourcesInit();
	void SpawnEnemy(float deltaTime);
	void Update(float deltaTime);
	void ScreenScroll(float deltaTime);
	void SceneChange(Scene _scene, float delayTime);
	void Release();

	GameManager();
	~GameManager();
};