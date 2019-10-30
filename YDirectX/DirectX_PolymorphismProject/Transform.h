#pragma once
#pragma once
#include<d3dx9.h>

enum DIRECTION
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};
class Transform
{
private:
	Transform* parent;
public:
	D3DXVECTOR3 sizeVector = {10, 10, 10};
	D3DXVECTOR3 rotateVector = {120, 40, 50};
	D3DXVECTOR3 transformVector = {0, 40, 0};

	D3DXMATRIXA16 sizeMat;
	D3DXMATRIXA16 rotateMatX;
	D3DXMATRIXA16 rotateMatY;
	D3DXMATRIXA16 rotateMatZ;
	D3DXMATRIXA16 transformMat;
	D3DXMATRIXA16 finalmat;

	D3DXVECTOR3 Forward;
	D3DXVECTOR3 Up;
	D3DXVECTOR3 Right;

	void Move(DIRECTION direction);
	void RotateY(float degree);

	void TransformUpdate();
	Transform(Transform* _parent = NULL);
	~Transform();
};