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
	ICE
};
//#include "BlockType.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define TILE_WIDTH_NUM 13
#define TILE_HEIGHT_NUM 13

using namespace std;

enum Scene
{
	TITLE,
	EDIT_MODE,
	INGAME
};

struct MapStackInfo
{
	POINT pt;
	BLOCKTYPE mapValue;
};
class GameManager
{
private:
	int score;
	int remains; //남은 플레이어 기회

	SpriteRenderer backGround;
	Tile blocks[15];
	static GameManager* mthis;
	//FontManager TitleFont;
	float Timer[10] = { 0 };
	float TimerReset[10] = { 0 };
	bool GameOverflag = false;
	const int GameOffsetX = 20;
	const int GameOffsetY = 20;
	stack<MapStackInfo> mapTileUndoStack;
	stack<MapStackInfo> mapTileRedoStack;
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };

	const int TileImageSizeX = 32;
	const int TileImageSizeY = 32;
public:
	bool isGameOver = false;

	//Scene scene = TITLE;
	Scene scene = EDIT_MODE;
	static GameManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new GameManager();
		return mthis;
	}
	void GameStart();
	void Draw(HDC hdc);
	void DrawTitle(HDC hdc);
	void DrawStagelogo(HDC hdc);
	void DrawBlack();
	void DrawTile(HDC hdc, BLOCKTYPE blockType, int x, int y);
	void MenuBlockDraw(HDC hdc);
	bool CollisionCheck();
	void Update(); //배경움직이고 등등
	void EditModeUpdate();
	void InTitleUpdate();
	void Init(HDC hdc, HINSTANCE _g_hInst);
	void Input(WPARAM wParam, LPARAM lParam, BLOCKTYPE radioButtonCheckNum);
	void CheckTileInput(int x, int y, BLOCKTYPE radioButtonCheckNum);
	void SetTimers();

	void LoadMap(int* mapValues);
	void SaveMap(string saveFileName);
	void Undo(); //ctrl z
	void Redo(); //ctrl y

	GameManager();
	~GameManager();
};
