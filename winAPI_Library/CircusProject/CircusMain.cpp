#include <windows.h>
#include"Player.h"
#include"GameManager.h"
#include "BitMap.h"
#include <crtdbg.h>
#include<iostream>
using namespace std;
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#pragma comment(lib, "msimg32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	srand(GetTickCount());
	return (int)Message.wParam;
}
enum TimerName
{
	TIMER_MOVINGTIMER,
	TIMER_MAKEFIRE,
	TIMER_SPRITECHANGE,
	TIMER_PLAYERSPRITECHANGE,
	TIMER_RESTART
};
enum KEYDOWN
{
	STOP_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static Player player;
	static KEYDOWN keydown;

	switch (iMessage)
	{
	case WM_CREATE:
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
		hdc = GetDC(hWnd);
		ResourceManager::GetInstance()->Init(hdc, g_hInst);
		GameManager::GetInstance()->MakeEnemyFire(NORMAL);
		SetTimer(hWnd, TIMER_MOVINGTIMER, 1, NULL);
		SetTimer(hWnd, TIMER_MAKEFIRE, 4000, NULL);
		SetTimer(hWnd, TIMER_SPRITECHANGE, 100, NULL);
		SetTimer(hWnd, TIMER_PLAYERSPRITECHANGE, 300, NULL);
		keydown = STOP_INPUT;
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_TIMER:
		switch ((TimerName)wParam)
		{
		case TIMER_MOVINGTIMER:
			if (GameManager::GetInstance()->isGameOver)
			{
				GameManager::GetInstance()->isGameOver = false;
				SetTimer(hWnd, TIMER_RESTART, 5000, NULL);
			}
			cout << GameManager::GetInstance()->distance << endl;
			switch (player.GetPlayerState())
			{
			case LEFT:
				GameManager::GetInstance()->distance += 3;
				GameManager::GetInstance()->MoveEnemyFire(-3);
				break;
			case RIGHT:
				GameManager::GetInstance()->distance -= 8;
				GameManager::GetInstance()->MoveEnemyFire(8);
				break;
			case PLAYER_DIE:
				return 0;
			default:
				break;
			}
			GameManager::GetInstance()->CollisionCheck(player.playerRECT, player);
			GameManager::GetInstance()->MoveEnemyFire(0);
			player.Jump();
			break;
		case TIMER_MAKEFIRE:
			GameManager::GetInstance()->MakeEnemyFire((FIRETYPE)(NORMAL + rand() % 3));
			break;
		case TIMER_PLAYERSPRITECHANGE:
			if (keydown == LEFT_INPUT)
			{
				player.Move(false);
			}
			else if (keydown == RIGHT_INPUT)
			{
				player.Move(true);
			}
			else
			{
				player.Stop();
			}
			break;
		case TIMER_SPRITECHANGE:
			if (keydown == LEFT_INPUT)
			{
				GameManager::GetInstance()->MoveBackground(2);
			}
			else if (keydown == RIGHT_INPUT)
			{
				GameManager::GetInstance()->MoveBackground(-2);
			}
			break;
		case TIMER_RESTART:
			GameManager::GetInstance()->Restart();
			KillTimer(hWnd, TIMER_RESTART);
			return 0;
		default:
			break;
		}

		InvalidateRect(hWnd, NULL, false); //다시그리는 거 WM_PAINT를 호출
		return 0;
	case  WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->DrawBackground(ResourceManager::backBuffer->GetmemDC());
		GameManager::GetInstance()->DrawFire(ResourceManager::backBuffer->GetmemDC());
		
		player.Draw(ResourceManager::backBuffer->GetmemDC());
		BitBlt(hdc, 0,0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			keydown = LEFT_INPUT;
			return 0;
		case VK_RIGHT:
			keydown = RIGHT_INPUT;
			return 0;
		case VK_SPACE:
			player.SetJump();
			return 0;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
			keydown = STOP_INPUT;
			break;
		}
		return 0;

	case WM_DESTROY:
		ResourceManager::GetInstance()->Release();
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
