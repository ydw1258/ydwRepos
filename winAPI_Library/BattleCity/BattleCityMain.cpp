#include <Windows.h>
#include <crtdbg.h>
#include <iostream>
#include "GameManager.h"
#include "RadioButtonID.h"

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
HWND r[15];

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
	int checkNum = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, NULL);
		//CreateWindow(TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 750, 10, 150, 700, hWnd, (HMENU)0, g_hInst, NULL);

		r[0] = CreateWindow("button", "Rectangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 720, 0, 120, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		for (int i = 1; i < 15; i++)
		{
			r[i] = CreateWindow("button", "Block", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 720, 40 * i, 120, 30, hWnd, (HMENU)i, g_hInst, NULL);
		}
		
		CreateWindow(TEXT("button"), TEXT("���� ����"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 900, 650, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("���� ����"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1000, 650, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		//CreateWindow(TEXT("button2"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		GameManager::GetInstance()->Init(hdc, g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) >= ID_R1 && LOWORD(wParam) <= ID_R15)
		{
			return 0;
		}
		switch (LOWORD(wParam))
		{
		case 0:
			//���� ����
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt";//Every File�� *.*, 
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = "c:\\";

			if (GetSaveFileName(&OFN))
			{
				sprintf(str, "%s ������ �����߽��ϴ�.", OFN.lpstrFile);
				MessageBox(hWnd, str, "���Ͽ��⼺��", MB_OK);
			}
			else
				MessageBox(hWnd, str, "���Ͽ������", MB_OK);

			break;
		case 1:
			//���� �ҷ�����
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt";//Every File�� *.*, 
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = "c:\\";
			break;
		}
		return 0;
	case WM_TIMER:
		GameManager::GetInstance()->Update();
		InvalidateRect(hWnd, NULL, false); //�ٽñ׸��� WM_PAINT�� ȣ��
		ReleaseDC(hWnd, hdc);
		return 0;
	case  WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw(hdc);
		
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
		
		for (int i = 0; i < 15; i++)
		{
			if (SendMessage(r[i], BM_GETCHECK, 0, 0) == BST_CHECKED) {
				checkNum = i;
				break;
			}
		}
		GameManager::GetInstance()->Input(wParam, lParam, checkNum);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
