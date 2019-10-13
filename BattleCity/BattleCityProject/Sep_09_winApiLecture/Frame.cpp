#include <windows.h>
#include <crtdbg.h>
#include<iostream>
#include<chrono>

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

	g_GameFrame.Init(hWnd);
	std::chrono
	//실제메인으로 사용.
	while (1)
	{
		if (PeekMessage(&Message, NULL, OU, OU, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			g_GameFrame.Update();
		}
	}


	srand(GetTickCount());
	return (int)Message.wParam;
}
const TCHAR *Items[] = { TEXT("APPLE"), TEXT("Orange"), TEXT("Melon"), TEXT("Grape"), TEXT("StrawBerry") };
HWND hCombo;


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[128];

	switch (iMessage)
	{
	case WM_CREATE: //p217
		
		SetTimer(hWnd, 1, 10, NULL);
		hdc = GetDC(hWnd);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
