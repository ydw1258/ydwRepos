#include"GameFrameWork.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";
GameFrameWork gameFrameWork;

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

	
	if (SUCCEEDED(gameFrameWork.InitD3D(hWnd)))
	{
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		

		while (GetMessage(&Message, NULL, 0, 0))
		{
			gameFrameWork.ProcessInputs(hWnd);
			gameFrameWork.Update();
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	gameFrameWork.CleanUp();

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_PAINT:
		
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0L);
			break;
		case '1':
			gameFrameWork.g_bWireframe = !gameFrameWork.g_bWireframe;
			break;
		case '2':
			gameFrameWork.g_bLockFrustum = !gameFrameWork.g_bLockFrustum;
			gameFrameWork.g_bHideFrustum = !gameFrameWork.g_bLockFrustum;
			break;
		}
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
