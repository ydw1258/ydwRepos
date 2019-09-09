#include <windows.h>
#include <crtdbg.h>
#include<iostream>
#include "GameManager.h"

/*
#include"Player.h"
#include"GameManager.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"
#include"Physics.h"
#include "BitMap.h" */
#define ID_R1 101
#define ID_R2 101
#define ID_R3 101
#define ID_R4 101
#define ID_R5 101
#define ID_R6 101
#define ID_R7 101
#define ID_R8 101
#define ID_R9 101
#define ID_R10 101
#define ID_R11 101
#define ID_R12 101
#define ID_R13 101
#define ID_R14 101
#define ID_R15 101

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
	//TCHAR str[128];
	OPENFILENAME OFN;
	char str[256];
	char lpstrFile[MAX_PATH] = "";
	hdc = GetDC(hWnd);

	HWND r[15];
	switch (iMessage)
	{
	case WM_CREATE: //p217
		SetTimer(hWnd, 1, 10, NULL);
		//CreateWindow(TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 750, 10, 150, 700, hWnd, (HMENU)0, g_hInst, NULL);

		r[0] = CreateWindow("button", "Rectangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 720, 0, 120, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		for (int i = 1; i < 14; i++)
		{
			r[i] = CreateWindow("button", "Block", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 720, 40 * i, 120, 30, hWnd, (HMENU)i, g_hInst, NULL);
		}
		
		CreateWindow(TEXT("button"), TEXT("파일 열기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 900, 650, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("파일 저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1000, 650, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		//CreateWindow(TEXT("button2"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		GameManager::GetInstance()->Init(hdc, g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			switch (LOWORD(wParam)) {
				MessageBox(hWnd, str, "라디오 버튼 클릭", MB_OK);
			case 0:
				break;
			}
			return 0;
		}

		switch (LOWORD(wParam))
		{
		case 0:
			//파일 열기
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt";//Every File을 *.*, 
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = "c:\\";

			if (GetSaveFileName(&OFN))
			{
				sprintf(str, "%s 파일을 선택했습니다.", OFN.lpstrFile);
				MessageBox(hWnd, str, "파일열기성공", MB_OK);
			}
			else
				MessageBox(hWnd, str, "파일열기실패", MB_OK);

			break;
		case 1:
			//파일 불러오기
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt";//Every File을 *.*, 
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = "c:\\";
			
			MessageBox(hWnd, TEXT("second button clicked"), TEXT("BUTTON"), MB_OK);
			break;
		}
		return 0;
	case WM_TIMER:
		GameManager::GetInstance()->Update();
		InvalidateRect(hWnd, NULL, false); //다시그리기 WM_PAINT를 호출
		ReleaseDC(hWnd, hdc);
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
