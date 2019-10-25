#pragma once
#pragma once
#include<d3dx9.h>

class Transform
{
public:
	D3DXVECTOR3 sv;
	D3DXVECTOR3 rv;
	D3DXVECTOR3 tv;

	D3DXMATRIXA16 s;
	D3DXMATRIXA16 rx;
	D3DXMATRIXA16 ry;
	D3DXMATRIXA16 rz;
	D3DXMATRIXA16 t;
	D3DXMATRIXA16 finalmat;


	void Update();
	Transform();
	~Transform();
};

