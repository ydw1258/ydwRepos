#include "..\BattleCityIngame\GameManager.h"
#include "GameManager.h"
#include<iostream>
#include<fstream>
using namespace std;

GameManager* GameManager::mthis = nullptr;

enum TIMERID
{
	TIMER_TITLESPRITECHAGE,
	TIMER_MAKEFIRE,
	TIMER_SPRITECHANGE,
	TIMER_GAMEOVERTIMER
};

void GameManager::Init(HDC hdc, HINSTANCE _g_hInst)
{
	BLOCKTYPE mapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM] = { EMPTY };

	for (int i = 0; i < 10; i++)
		Timer[i] = 0;
	//player.Init(3); // playerSpeed
	TitleFont.Init();
	ResourceManager::GetInstance()->Init(hdc, _g_hInst);
	SetTimers();

	for (int i = 0; i < 6; i++)
		blocks[i].Init(16, 16, (IMAGENUM)(IMAGENUM_EMPTY + i));
}
void GameManager::Input(WPARAM wParam, LPARAM lParam, BLOCKTYPE radioButtonCheckNum)
{
	CheckTileInput(LOWORD(lParam), HIWORD(lParam), radioButtonCheckNum);
}
void GameManager::Shot()
{
	if(bullets.size)
}
void GameManager::CheckTileInput(int x, int y, BLOCKTYPE radioButtonCheckNum)
{
	RECT rect;
	POINT pt;
	pt.x = x;
	pt.y = y;

	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			rect.top = i * TileImageSizeY + GameOffsetY;
			rect.bottom = (i + 1) * TileImageSizeY + GameOffsetY;
			rect.left = j * TileImageSizeX + GameOffsetX;
			rect.right = (j + 1) * TileImageSizeX  + GameOffsetX;

			//cout << rect.top << " " << rect.bottom << " " << rect.left << " " << rect.right << endl;
			if (Physics::GetInstance()->RECTbyPointCollisionCheck(rect, pt))
			{
				if (j == 4 && i == 12 || j == 5 && i == 12 || j == 6 && i == 12 || j == 7 && i == 12 ||
					j == 0 && i == 0 || j == 6 && i == 0 || j == 12 && i == 0)
					return;
				MapStackInfo mapStackInfo;
				mapStackInfo.pt.x = j;
				mapStackInfo.pt.y = i;
				mapStackInfo.mapValue = mapTile[i * TILE_WIDTH_NUM + j];
				mapTileUndoStack.push(mapStackInfo);

				//라디오 버튼 클릭된 것
				mapTile[i * TILE_WIDTH_NUM + j] = radioButtonCheckNum;
					//SendMessage(r1, BM_GETCHECK, 0, 0) == BST_CHECKED) GRAPH = 0;
			}
		}
	}
	
}
void GameManager::SetTimers()
{
	TimerReset[TIMER_MAKEFIRE] = 5.0f;
	TimerReset[TIMER_SPRITECHANGE] = 0.4f;
	TimerReset[TIMER_TITLESPRITECHAGE] = 0.5f;
	TimerReset[TIMER_GAMEOVERTIMER] = 2.0f;
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
void GameManager::SaveMap(string saveFileName)
{
	string out_line;
	ofstream out(saveFileName);

	for (int i = 0; i < TILE_HEIGHT_NUM; i++)
	{
		for (int j = 0; j < TILE_WIDTH_NUM; j++)
		{
			out_line = to_string(mapTile[i * TILE_HEIGHT_NUM + j]) + "\n";
			out << out_line;
		}
	}


	out.close();
}
void GameManager::Undo()
{
	BLOCKTYPE tempMapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM];

	if (mapTileUndoStack.empty())
		return;

	//현재상태를 Redo스택에 저장
	MapStackInfo tempRedo;

	tempRedo.pt.x = mapTileUndoStack.top().pt.x;
	tempRedo.pt.y = mapTileUndoStack.top().pt.y;
	tempRedo.mapValue = mapTile[mapTileUndoStack.top().pt.y * TILE_HEIGHT_NUM + mapTileUndoStack.top().pt.x];
	mapTileRedoStack.push(tempRedo);
	cout << tempRedo.mapValue << endl;

	//Undo 스택에서 가져오기
	mapTile[mapTileUndoStack.top().pt.y * TILE_HEIGHT_NUM + mapTileUndoStack.top().pt.x] = mapTileUndoStack.top().mapValue;
	mapTileUndoStack.pop();
}
void GameManager::Redo()
{
	BLOCKTYPE tempMapTile[TILE_HEIGHT_NUM * TILE_WIDTH_NUM];
	if (mapTileRedoStack.empty())
		return;

	//현재상태를 Undo스택에 저장
	MapStackInfo tempUndo;

	tempUndo.pt.x = mapTileRedoStack.top().pt.x;
	tempUndo.pt.y = mapTileRedoStack.top().pt.y;
	tempUndo.mapValue = mapTile[tempUndo.pt.y * TILE_HEIGHT_NUM + tempUndo.pt.x];

	mapTileUndoStack.push(tempUndo);

	//Redo 스택에서 가져오기
	mapTile[mapTileRedoStack.top().pt.y * TILE_HEIGHT_NUM + mapTileRedoStack.top().pt.x] = mapTileRedoStack.top().mapValue;
	mapTileRedoStack.pop();
	
}
//Update 매프레임 호출
void GameManager::Update()
{
	Physics::GetInstance()->deltaTimeInit();
	EditModeUpdate();
}
void GameManager::InTitleUpdate()
{
	Timer[TIMER_TITLESPRITECHAGE] += Physics::GetInstance()->deltaTime;

	if (Timer[TIMER_TITLESPRITECHAGE] >= TimerReset[TIMER_TITLESPRITECHAGE])
	{
		Timer[TIMER_TITLESPRITECHAGE] = 0;
	}
}
void GameManager::EditModeUpdate(){}

//Draw 매프레임 호출
void GameManager::Draw(HDC hdc)
{
	ResourceManager::GetInstance()->Draw(hdc, 0, 0, 600, 600, IMAGENUM_BLACKBACKGROUND);
	MenuBlockDraw(hdc);
	
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
void GameManager::MenuBlockDraw(HDC hdc)
{
	/*"BattleResource/__empty.bmp"
	"BattleResource/__brick.bmp",
	"BattleResource/__block.bmp",
	"BattleResource/__water.bmp",
	"BattleResource/__bush.bmp",
	"BattleResource/__steel.bmp", */

	const int MINIBLOCKSIZE = 16;
	int x = 680;
	int y = 10;
	int yInterval = 40;

	for (int i = 0; i < 6; i++)
	{
		if (i == 1 || i == 2)//brick, block
		{
			blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y); // up right
			blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y + MINIBLOCKSIZE); //down right
			y += yInterval;
			blocks[i].Draw(hdc, x, y + MINIBLOCKSIZE);  //down left
			blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y + MINIBLOCKSIZE); //down right
			y += yInterval;
			blocks[i].Draw(hdc, x, y); //up left
			blocks[i].Draw(hdc, x, y + MINIBLOCKSIZE);  //down left
			y += yInterval;
			blocks[i].Draw(hdc, x, y); //up left
			blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y); // up right
			y += yInterval;
		}
		
		blocks[i].Draw(hdc, x, y); //up left
		blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y); // up right
		blocks[i].Draw(hdc, x, y + MINIBLOCKSIZE);  //down left
		blocks[i].Draw(hdc, x + MINIBLOCKSIZE, y + MINIBLOCKSIZE); //down right
		y += yInterval;
	}
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