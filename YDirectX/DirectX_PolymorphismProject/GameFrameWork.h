#pragma once
#include "SAFE_DELETE.h"
#include "Triangle.h"
#include "Cube.h"
#include "ZCamera.h"
//#include "ZFLog.h" //잠시 보류
#include "CUSTOM_DATATYPES.h"
#include "ZCamera.h"
#include "Ground.h"

//dx 그래픽 카드에 명령
//a 그릴 옵션 a draw b 그릴 옵션 b draw

enum Scene
{
	UNIVERSE,
	CUBE_RENDER,
	TRIANGLE,
	TERRAIN
};
#define BMP_HEIGHTMAP	"map128.bmp"

class GameFrameWork
{
private:
	Scene scene = UNIVERSE;

	HWND						hWnd = NULL;
	LPDIRECT3D9					g_pD3D = NULL;

	Cube cube;
	/*
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
	LPDIRECT3DTEXTURE9			g_pTexHeight = NULL;
	LPDIRECT3DTEXTURE9			g_pTexDiffuse = NULL;*/

	//D3DXMATRIXA16				g_matAni;

	DWORD						g_dwMouseX = 0;
	DWORD						g_dwMouseY = 0;
	Ground ground;


	DWORD						g_cxHeight = 0;
	DWORD						g_czHeight = 0;
	
public:
	LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
	HRESULT InitD3D(HWND& hWnd);
	
	void CleanUp();
	void Update();
	void Render();

	void ProcessInputs();
	void ProcessMouse();
	void ProcessKey();

	//프러스텀 컬링
	BOOL					g_bWireframe = FALSE;	// 와이어프레임으로 그릴것인가?
	BOOL					g_bHideFrustum = TRUE;	// Frustum을 안그릴 것인가?
	BOOL					g_bLockFrustum = FALSE;	// Frustum을 고정할 것인가?
	void Init();
	void SetupCamera();
	
	VOID Animate();
	VOID SetupLights();
	//void LogStatus(void);
	
	GameFrameWork();
	~GameFrameWork();

	HRESULT InitGeometry();
	
};