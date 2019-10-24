#include <d3dx9.h>
#include <mmsystem.h>

#include "MainMatrix.h"
#include "SAFE_DELETE.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND g_hWnd;
char g_szClassName[256] = "Hello World!!";


MVector4 TV1 = { -50.0f, .0f, .0f };
MVector4 TV2 = { 50.0f, .0f, .0f };
MVector4 TV3 = { .0f, -50.0f, .0f };

void Test()
{
	//렌더링 파이프 라인
	//로컬스페이스, 
	//

	MMatrix Test1;
	Test1.V1 = { 1.0f, 2.0f, 3.0f, 4.0f };
	Test1.V2 = { 2.0f, 3.0f, 4.0f, 5.0f };
	Test1.V3 = { 3.0f, 4.0f, 5.0f, 6.0f };
	Test1.V4 = { 4.0f, 5.0f, 6.0f, 7.0f };
	
	MMatrix Test2;
	Test2.V1 = { 1.0f, 2.0f, 3.0f, 4.0f };
	Test2.V2 = { 2.0f, 3.0f, 4.0f, 5.0f };
	Test2.V3 = { 3.0f, 4.0f, 5.0f, 6.0f };
	Test2.V4 = { 4.0f, 5.0f, 6.0f, 7.0f };

	MMatrix T3 = Test1 * Test2;
	T3 = T3.Translation({ 1.0f, 2.0f, 3.0f , .0f});


	// T3 = T3.RotX(60.0f);
	// T3 = T3.RotY(60.0f);
	T3 = T3.RotZ(60.0f);
	// T3 = T3.Scale(Test1.V1);
	   
	D3DXMATRIXA16 A1 =
	{ 1.0f, 2.0f, 3.0f, 4.0f,
	 2.0f, 3.0f, 4.0f, 5.0f,
	 3.0f, 4.0f, 5.0f, 6.0f,
	 4.0f, 5.0f, 6.0f, 7.0f, };

	D3DXMATRIXA16 A2 =
	{ 1.0f, 2.0f, 3.0f, 4.0f,
	 2.0f, 3.0f, 4.0f, 5.0f,
	 3.0f, 4.0f, 5.0f, 6.0f,
	 4.0f, 5.0f, 6.0f, 7.0f, };
	
	D3DXMATRIXA16 A3;
	D3DXMatrixMultiply(&A3, &A1, &A2);
	D3DXMatrixTranslation(&A3, 1.0f, 2.0f, 3.0f);
	
	// D3DXMatrixRotationX(&A3, 60.0f);
	// D3DXMatrixRotationY(&A3, 60.0f);
	D3DXMatrixRotationZ(&A3, 60.0f);

	D3DXMatrixScaling(&A3, Test1.V1.x, Test1.V1.y, Test1.V1.z);


	int a = 0;
}


void init_vertex()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	/*
	TV1.x += rc.right * .5f;
	TV2.x += rc.right * .5f;
	TV3.x += rc.right * .5f;
	*/

	//TV1.y += rc.bottom * .5f;
	//TV2.y += rc.bottom * .5f;
	//TV3.y += rc.bottom * .5f;
}

void rotating()
{
	UINT iTime = timeGetTime() % 1000;
	float fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	MMatrix Rot;
	MMatrix TP;
	
	Rot.RotY(fAngle);
	TP.identity(); 
	TP.proj(1.0f, .4f, .5f, 1000.0f);

	MMatrix Temp = (Rot * TP);

	//각 변들과 변환된 행렬 곱
	MVector4 T1 = Temp.VecMultiMat(TV1);
	MVector4 T2 = Temp.VecMultiMat(TV2);
	MVector4 T3 = Temp.VecMultiMat(TV3);
	
	//윈도우 좌표계로 이동 화면 중심을 0, 0으로
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	T1.x += rc.right * .5f;
	T2.x += rc.right * .5f;
	T3.x += rc.right * .5f;	

	T1.y += rc.bottom * .5f;
	T2.y += rc.bottom * .5f;
	T3.y += rc.bottom * .5f;

	PAINTSTRUCT Rc;
	HDC hdc = BeginPaint(g_hWnd, &Rc);

	MoveToEx(hdc, T1.x, T1.y, NULL);
	LineTo(hdc, T2.x, T2.y);

	MoveToEx(hdc, T2.x, T2.y, NULL);
	LineTo(hdc, T3.x, T3.y);

	MoveToEx(hdc, T3.x, T3.y, NULL);
	LineTo(hdc, T1.x, T1.y);

	EndPaint(g_hWnd, &Rc);
}


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
	g_hWnd = hWnd;

	// Test();
	init_vertex();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ZeroMemory(&Message, sizeof(Message));
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, NULL);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		rotating();
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}