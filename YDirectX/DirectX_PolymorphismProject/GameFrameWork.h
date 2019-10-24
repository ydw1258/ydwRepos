#pragma once
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Triangle.h"
#include "SpinTriangle.h"
//dx �׷��� ī�忡 ���
//a �׸� �ɼ� a draw b �׸� �ɼ� b draw

class GameFrameWork
{
private:
	LPDIRECT3D9			g_pD3D = NULL;
	LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;
	Triangle tri;
	SpinTriangle tri2;
public:
	HRESULT InitD3D(HWND hWnd);
	
	void CleanUp();
	void Render();
	GameFrameWork();
	~GameFrameWork();
};