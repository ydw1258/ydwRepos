#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include <map>
using namespace std;

#define WINSIZE_X		800
#define WINSIZE_Y		600

class Timer;
class ASEMesh;
class Camera;
class GameManager
{
private:
	static GameManager*			m_pThis;
	GameManager();

	

	LPDIRECT3D9					m_pD3d;
	LPDIRECT3DDEVICE9			m_pDevice;
	bool						m_bWireFrame;

	Camera*						m_pCamera;
	Timer*						m_pTimer;
	ASEMesh*					m_pMesh;
public:
	bool						m_bBoneView;
	bool						m_bMeshView;

	static GameManager*			GetInstance()
	{
		if (m_pThis == nullptr)
		{
			m_pThis = new GameManager();
		}

		return m_pThis;
	}

	HRESULT						CreateDevice(HWND hWnd);
	void						InitLight();

	HRESULT						Init(HWND hWnd, HINSTANCE hInst);
	void						Update();
	void						Render();
	void						Release();

	
	~GameManager();
};

