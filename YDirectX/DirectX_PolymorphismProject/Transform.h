#pragma once
#pragma once
#include<d3dx9.h>

class Transform
{
public:
	D3DXVECTOR3 sv = {1, 1, 1};
	D3DXVECTOR3 rv = {0, 0, 0};
	D3DXVECTOR3 tv = {0, 0, 0};

	D3DXMATRIXA16 s;
	D3DXMATRIXA16 rx;
	D3DXMATRIXA16 ry;
	D3DXMATRIXA16 rz;
	D3DXMATRIXA16 t;
	D3DXMATRIXA16 finalmat;


	void TransformUpdate();
	Transform();
	~Transform();
};

