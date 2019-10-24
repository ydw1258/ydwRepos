#include "EXCamera.h"


#define FASTSPEED .1f;
#define SLOWSPEED .001f;


EXCamera::EXCamera() :
	CamEyept({ .0f, 0.0f, -5.0f }),
	CamLookAt({.0f, .0f, .0f})
{
}


EXCamera::~EXCamera()
{
}



void EXCamera::update()
{
	update_input();
}


void EXCamera::update_input()
{
	if (0 != GetAsyncKeyState(VK_UP))
	{
		CamEyept.y += FASTSPEED;
	}
	else if(0 != GetAsyncKeyState(VK_DOWN))
	{
		CamEyept.y -= FASTSPEED;
	}
	else if (0 != GetAsyncKeyState(VK_LEFT))
	{
		CamEyept.x -= FASTSPEED;
	}
	else if (0 != GetAsyncKeyState(VK_RIGHT))
	{
		CamEyept.x += FASTSPEED;
	}
	else if (0 != GetAsyncKeyState(VK_LSHIFT))
	{
		CamEyept.z -= FASTSPEED;
	}
	else if (0 != GetAsyncKeyState(VK_LCONTROL))
	{
		CamEyept.z += FASTSPEED;
	}
}

void EXCamera::update_trans(LPDIRECT3DDEVICE9 _Device)
{
	// 0, 1, 0 으로 고정 시킨다.
	D3DXVECTOR3 vUpVec(.0f, 1.0f, .0f);

	CamLookAt = CamEyept;
	CamLookAt.z = 5.0f;


	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &CamEyept, &CamLookAt, &vUpVec);
	_Device->SetTransform(D3DTS_VIEW, &matView);


	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	_Device->SetTransform(D3DTS_PROJECTION, &matProj);
}