#pragma once
#include <d3dx9.h>



class EXCamera
{
public:
	EXCamera();
	~EXCamera();


private:
	D3DXVECTOR3 CamEyept;
	D3DXVECTOR3 CamLookAt;


	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;

public:
	void update();
	void update_trans(LPDIRECT3DDEVICE9 _Device);

private:
	void update_input();
};

