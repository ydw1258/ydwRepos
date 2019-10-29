#include <d3d9.h>
#include <d3dx9.h>
#include "ZFLog.h"
#include "ZCamera.h"
#include "SAFE_DELETE.h"
#include "GameFrameWork.h"

#define WINDOW_W		500
#define WINDOW_H		500
#define WINDOW_TITLE	"HeightMap-TList"
#define BMP_HEIGHTMAP	"map128.bmp"
GameFrameWork gameFrameWork;

char g_szClassName[256] = "BasicFrame";
HWND g_hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		gameFrameWork.CleanUp();
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostMessage(hWnd, WM_DESTROY, 0, 0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;

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
	
	g_hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	gameFrameWork.InitD3D(g_hWnd);
	gameFrameWork.Init();
	
	//다그리고 마지막에 카메라 셋업
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	while (true)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			gameFrameWork.Update();
			
		}

	}

	ZFLog::Release();

	UnregisterClass(g_szClassName, hInstance);
	return (int)Message.wParam;
}