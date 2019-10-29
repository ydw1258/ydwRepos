#pragma once
#include "SAFE_DELETE.h"
#include "Triangle.h"
#include "Cube.h"
#include "ZCamera.h"
//#include "ZFLog.h" //��� ����
#include "CUSTOM_DATATYPES.h"
#include "ZCamera.h"
#include "Ground.h"

//dx �׷��� ī�忡 ���
//a �׸� �ɼ� a draw b �׸� �ɼ� b draw

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

	//�������� �ø�
	BOOL					g_bWireframe = FALSE;	// ���̾����������� �׸����ΰ�?
	BOOL					g_bHideFrustum = TRUE;	// Frustum�� �ȱ׸� ���ΰ�?
	BOOL					g_bLockFrustum = FALSE;	// Frustum�� ������ ���ΰ�?
	void Init();
	void SetupCamera();
	
	VOID Animate();
	VOID SetupLights();
	//void LogStatus(void);
	
	GameFrameWork();
	~GameFrameWork();

	HRESULT InitGeometry();
	
};