#include "Transform.h"

void Transform::Update()
{
	float x, y, z;
	D3DXQUATERNION quat;

	D3DXMatrixScaling(&s, sv.x, sv.y, sv.z);
	D3DXMatrixRotationX(&rx, rv.x);
	D3DXMatrixRotationY(&ry, rv.y);
	D3DXMatrixRotationZ(&rz, rv.z);
	D3DXMatrixTranslation(&t, tv.x, tv.y, tv.z);

	finalmat = s * rx * ry * rz * t;
}

Transform::Transform()
{
}


Transform::~Transform()
{
}