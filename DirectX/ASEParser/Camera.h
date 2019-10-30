#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
using namespace std;

class Camera
{
private:
	D3DXVECTOR3			m_vEye;
	D3DXVECTOR3			m_vLook;

	D3DXMATRIXA16		m_matView;
	D3DXMATRIXA16		m_matProj;
	D3DXMATRIXA16		m_matViewProj;

	float				m_fFOV;
	float				m_fAspect;
	float				m_fNear;
	float				m_fFar;
	float				m_fCamMoveSpeed;

protected:
	void	Foward(LPDIRECT3DDEVICE9 pDevice ,float fOffset);
	void	Side(LPDIRECT3DDEVICE9 pDevice ,float fOffset);
	void	Fan(LPDIRECT3DDEVICE9 pDevice ,float fOffset);
	void	Tilt(LPDIRECT3DDEVICE9 pDevice ,float fOffset);
	void	UpDown(LPDIRECT3DDEVICE9 pDevice ,float fOffset);

	void	SetView(LPDIRECT3DDEVICE9 pDevice);
	void	SetProj(LPDIRECT3DDEVICE9 pDevice);

public:
	Camera();
	~Camera();

	void	Init(LPDIRECT3DDEVICE9 pDevice,D3DXVECTOR3 vEye, D3DXVECTOR3 vLook, float fFOV, float fAspect, float fNear, float fFar);
	void	Update(LPDIRECT3DDEVICE9 pDevice , float fEllipseTime);
	void	Render();

	void	SetEye(D3DXVECTOR3 vEye) { m_vEye = vEye; }
	void	SetLook(D3DXVECTOR3 vLook) { m_vLook = vLook; }
};

