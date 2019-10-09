#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include "GameFrameWork.h"
#include "ResourceManager.h"
#include "PacketManager.h"
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWnd;
LPCTSTR lpszClass = TEXT("OmokClient");
GameFrameWork g_GameFrame;

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32")
//#pragma comment(lib, "StaticWinApiLib.lib")

#if _DEBUG

#if x64
#pragma comment(lib, "StaticWinApiLibDebugx64")
#else
#pragma comment(lib, "StaticWinApiLibDebugWin32")
#endif // x64
#else

#if x64
#pragma comment(lib, "StaticWinApiLibReleasex64")
#else
#pragma comment(lib, "StaticWinApiLibReleaseWin32")
#endif // x64

#endif // _DEBUG

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	g_GameFrame.Init(hWnd, g_hInst);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		//if(GetMessage(&Message, NULL, 0, 0))
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
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
	
	 g_GameFrame.Release();

	//closesocket();
	WSACleanup();

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int x;
	static int y;
	static BOOL bnowDraw = FALSE;

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SOCKET:
		PacketManager::GetInstance()->ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		InvalidateRect(hWnd, NULL, false);
		return 0;
	case WM_MOUSEMOVE:
		if (bnowDraw == TRUE) {
			if (GameManager::GetInstance()->scene != PLAYING || !PacketManager::GetInstance()->isMyTurn())
				return 0;
			
			hdc = GetDC(hWnd);
			DRAWPT pt;
			pt.startX = x;
			pt.startY = y;
			
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			pt.endX = x;
			pt.endY = y;
			POINT checkPt; checkPt.x = x; checkPt.y = y;
			if (Physics::GetInstance()->RECTbyPointCollisionCheck(GameManager::GetInstance()->whiteBoard, checkPt))
			{
				PacketManager::GetInstance()->SendPos(pt);
			}
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bnowDraw = FALSE;
		return 0;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		bnowDraw = TRUE;
		return 0;
	default:
		return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
}