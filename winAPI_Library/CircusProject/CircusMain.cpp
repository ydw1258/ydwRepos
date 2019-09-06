#include <windows.h>
#include"Player.h"
#include"GameManager.h"
#include"SpriteRenderer.h"
#include"Physics.h"
#include "BitMap.h"
#include <crtdbg.h>
#include<iostream>
using namespace std;
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
		SetTimer(hWnd, 1, 10, NULL);
		hdc = GetDC(hWnd);
		GameManager::GetInstance()->Init(hdc, g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_TIMER:
		GameManager::GetInstance()->Update();
		InvalidateRect(hWnd, NULL, false); //다시그리기 WM_PAINT를 호출
		return 0;
	case  WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			GameManager::GetInstance()->distance += 3;
			GameManager::GetInstance()->MoveEnemyFire(-3);
		case VK_RIGHT:
			GameManager::GetInstance()->distance -= 8;
			GameManager::GetInstance()->MoveEnemyFire(8);
			return 0;
		case VK_SPACE:
			//player.SetJump();
			return 0;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
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
