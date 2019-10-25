#pragma once
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Triangle.h"
#include "SpinTriangle.h"
#include "Cube.h"
#include "ZCamera.h"


//dx 그래픽 카드에 명령
//a 그릴 옵션 a draw b 그릴 옵션 b draw

enum Scene
{
	UNIVERSE,
	CUBE_RENDER,
	TRIANGLE,
	TERRAIN
};
class GameFrameWork
{
private:
	LPDIRECT3D9			g_pD3D = NULL;
	LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;
	
	Scene scene = UNIVERSE;

	Triangle tri;
	SpinTriangle tri2;
	Cube cube;

	
	
public:
	HRESULT InitD3D(HWND hWnd);
	
	void CleanUp();
	void Update();
	void Render();

	DWORD g_dwMouseX = 0;
	DWORD g_dwMouseY = 0;
	void ProcessInputs(HWND hwnd);
	void ProcessMouse(HWND g_hwnd);
	void ProcessKey();

	//프러스텀 컬링
	BOOL					g_bWireframe = FALSE;	// 와이어프레임으로 그릴것인가?
	BOOL					g_bHideFrustum = TRUE;	// Frustum을 안그릴 것인가?
	BOOL					g_bLockFrustum = FALSE;	// Frustum을 고정할 것인가?
	//HRESULT ProcessFrustumCull();

	VOID Animate();
	VOID SetupLights();
	void LogFPS(void);
	void LogStatus(void);

	GameFrameWork();
	~GameFrameWork();
	
};