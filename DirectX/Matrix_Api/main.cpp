#include <Windows.h>
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include<math.h>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWnd;
LPCTSTR lpszClass = TEXT("Hello World!!");

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

struct CUSTOMVECTEX
{
	float x, y, z, w; //행벡터로 일단 처리
	//DWORD color;
};

struct Metrices4X4
{
	float _11, _12, _13, _14, //_11, _12, _13, _14,
		_21, _22, _23, _24,   //_21, _22, _23, _24,
		_31, _32, _33, _34,   //_31, _32, _33, _34,
		_41, _42, _43, _44;   //_41, _42, _43, _44;
};

Metrices4X4 PruductMatbyMat(Metrices4X4 &mat1, Metrices4X4 &mat2);
void ProductMatbyVector(Metrices4X4 &mat, CUSTOMVECTEX &vec);
Metrices4X4 MoveMat(CUSTOMVECTEX &vec);
Metrices4X4 RotationMatX(float angle);
Metrices4X4 RotationMatY(float angle);
Metrices4X4 RotationMatZ(float angle);
void ScalingMat(Metrices4X4 &mat, float TransSize);
void printMat(Metrices4X4 &mat);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;

void rotating()
{
	UINT iTime = timeGetTime() % 1000;
	float fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;

	MMatrix Rot;
	MMatrix TP;

	Rot.RotY(fAngle);
	TP.identity();
	TP.proj(1000.0f, 600.0f, .5f, 1000.0f);

	MVector4 T1 = Rot * TV1;
	MVector4 T2 = Rot * TV2;
	MVector4 T3 = Rot * TV3;



	PAINTSTRUCT Rc;

	HDC hdc = BeginPaint(g_hWnd, &Rc);
	MoveToEx(hdc, T1.x + rc.right * .5f, T1.y, NULL);
	LineTo(hdc, T2.x + rc.right * .5f, T2.y);

	MoveToEx(hdc, T2.x + rc.right * .5f, T2.y, NULL);
	LineTo(hdc, T3.x + rc.right * .5f, T3.y);

	MoveToEx(hdc, T3.x + rc.right * .5f, T3.y, NULL);
	LineTo(hdc, T1.x + rc.right * .5f, T1.y);
	EndPaint(g_hWnd, &Rc);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
int cameraX = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	switch (iMessage)
	{
	case WM_KEYDOWN:
		return 0;
	case WM_CREATE: //윈도우 생성시 호출 초기화에 사용
	{
		Metrices4X4 mat1 = { 1, 2, 3, 4,
								2, 1, 3, 1,
								4, 1, 2, 1,
								5, 0, 2, 1 };

		Metrices4X4 mat2 = { 1, 2, 3, 4, 0, 1, 1, 1, 2, 1, 2, 1, 1, 0, 2, 1 };
		//회전 행렬
		
		Metrices4X4 resultMat = PruductMatbyMat(mat1, mat2);
		printMat(resultMat);
	}
		return 0;
	case WM_TIMER:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MoveToEx(hdc, 0, 100, NULL);
		LineTo(hdc, 200, 100);
		LineTo(hdc, 100, 0);
		LineTo(hdc, 0, 100);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//매트릭스 x 매트릭스
Metrices4X4 PruductMatbyMat(Metrices4X4 &mat1, Metrices4X4 &mat2)
{
	Metrices4X4 resultMat;

	resultMat = { mat1._11 * mat2._11, mat1._12 * mat2._21, mat1._13 * mat2._31, mat1._14 * mat2._41,
		mat1._21 * mat2._12, mat1._22 * mat2._22, mat1._23 * mat2._32, mat1._24 * mat2._42,
		mat1._31 * mat2._13, mat1._32 * mat2._23, mat1._33 * mat2._33, mat1._34 * mat2._43,
		mat1._41 * mat2._14, mat1._42 * mat2._24, mat1._43 * mat2._34, mat1._44 * mat2._44,
	};
	return resultMat;
}
//매트릭스 x 벡터

void ProductMatbyVector(Metrices4X4 &mat, CUSTOMVECTEX &vec)
{
	CUSTOMVECTEX resultVec =
	{
		/*mat._11 * vec.x + mat._21 * vec.y + mat._31 * vec.z, mat._41 * vec.w,
		mat._12 * vec.x + mat._22 * vec.y + mat._32 * vec.z + mat._42 * vec.w,
		 mat._13 * vec.x + mat._23 * vec.y + mat._33 * vec.z + mat._43 * vec.w,
		 mat._14 * vec.x + mat._24 * vec.y + mat._34 * vec.z + mat._44 * vec.w*/
	};
}

Metrices4X4 MoveMat(CUSTOMVECTEX &vec)
{
	//이동 행렬
	//단위 행렬 함수
	Metrices4X4 resultMat = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		vec.x, vec.y, vec.z, 1
	};
	return resultMat;
}

Metrices4X4 RotationMatX(float angle)
{
	//중심이 되는 축의 값은 변하지 않음
	//x
	Metrices4X4 resultMat = { 1, 0, 0, 0,
		0, cos(angle), sin(angle), 0,
		0, -sin(angle), cos(angle), 0,
		0, 0, 0, 1
	};

	return resultMat;
}
//y
Metrices4X4 RotationMatY(float angle)
{
	Metrices4X4 resultMat = { cos(angle), 0, -sin(angle), 0,
	0, 1, 0, 0,
	sin(angle), 0, cos(angle), 0,
	0, 0, 0,1
	};

	return resultMat;
}
Metrices4X4 RotationMatZ(float angle)
{
	Metrices4X4 resultMat = { cos(angle), sin(angle), 0, 0,
	-sin(angle), cos(angle), 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};
	return resultMat;
}
void ScalingMat(Metrices4X4 &mat, float TransSize)
{
	mat._11 = mat._11 * TransSize;
	mat._22 = mat._22 * TransSize;
	mat._33 = mat._33 * TransSize;
}
void printMat(Metrices4X4 &mat)
{
	cout << mat._11 << " " << mat._12 << " " << mat._13 << " " << mat._14 << endl;
	cout << mat._21 << " " << mat._22 << " " << mat._23 << " " << mat._24 << endl;
	cout << mat._31 << " " << mat._32 << " " << mat._33 << " " << mat._34 << endl;
	cout << mat._41 << " " << mat._42 << " " << mat._43 << " " << mat._44 << endl;
}

void SetupMatreices()
{
	// 2n / w ,0, 0, 0
	//0, 2n / h, 0, 0
	//0, 0, f / f - n, 1
	//0, 0, fn / f - n, 0

	/*
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyept(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLootatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;

	D3DXMatrixLookAtLH(&matView, &vEyept, &vLootatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);*/
}