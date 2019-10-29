#include "Transform.h"

void Transform::Move(DIRECTION direction)
{
	switch (direction)
	{
	case FRONT:

		break;
	case BACK:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	default:
		break;
	}
}

void Transform::RotateY(DIRECTION direction)
{
	switch (direction)
	{
	case LEFT:
		break;
	case RIGHT:
		break;
	default:
		break;
	}
}

void Transform::TransformUpdate()
{
	float x, y, z;
	D3DXQUATERNION quat;

	D3DXMatrixScaling(&sizeMat, sizeVector.x, sizeVector.y, sizeVector.z);
	D3DXMatrixRotationX(&rotateMatX, rotateVector.x * D3DX_PI / 180.0f);
	D3DXMatrixRotationY(&rotateMatY, rotateVector.y * D3DX_PI / 180.0f);
	D3DXMatrixRotationZ(&rotateMatZ, rotateVector.z * D3DX_PI / 180.0f);
	D3DXMatrixTranslation(&transformMat, transformVector.x, transformVector.y, transformVector.z);

	// 기저벡터
	// 회전후 나아갈 방향에 필요하기 때문에 별개로 저장
	D3DXVECTOR3 tForward = { .0f, .0f, 1.0f };
	D3DXVECTOR3 tUp = { .0f, 1.0f, .0f };
	D3DXVECTOR3 tRight = { 1.0f, .0f, .0f };
	
	D3DXMATRIXA16 rotmat = rotateMatX * rotateMatY * rotateMatZ;
	D3DXVec3TransformNormal(&Forward, &tForward, &rotmat);
	D3DXVec3TransformNormal(&Up, &tUp, &rotmat);
	D3DXVec3TransformNormal(&Right, &tRight, &rotmat);

	D3DXVec3Normalize(&Forward, &Forward);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Normalize(&Right, &Right);

	//vertex 정점 전부에 finalmat을 곱함
	finalmat = sizeMat * rotmat * transformMat;
}

Transform::Transform()
{
	D3DXMatrixIdentity(&sizeMat);
	D3DXMatrixIdentity(&rotateMatX);
	D3DXMatrixIdentity(&rotateMatY);
	D3DXMatrixIdentity(&rotateMatZ);
	D3DXMatrixIdentity(&transformMat);
	D3DXMatrixIdentity(&finalmat);
}


Transform::~Transform()
{
}
