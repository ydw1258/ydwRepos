#include <windows.h>
#include <crtdbg.h> //메모리릭체크
#include<iostream>

/*
#include"Player.h"
#include"GameManager.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"
#include"Physics.h"
#include "BitMap.h" */

using namespace std;
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//메모리릭체크
	//_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
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
		//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

		SetTimer(hWnd, 1, 10, NULL);
		hdc = GetDC(hWnd);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_TIMER:
		//GameManager::GetInstance()->distance += 3;
		InvalidateRect(hWnd, NULL, false); //다시그리기 WM_PAINT를 호출
		return 0;
	case  WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return 0;
		case VK_RIGHT:
			return 0;
		case VK_SPACE:
			return 0;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		}
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
