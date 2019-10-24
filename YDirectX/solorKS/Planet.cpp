#include "Planet.h"



Planet::Planet() : MyParent(nullptr)
{
}


Planet::~Planet()
{
}

void Planet::animate()
{
	float x, y, z;
	D3DXQUATERNION quat;

	D3DXMatrixRotationY(&MyRotMat, GetTickCount() * MyAngleSpeed / 50000.0f);
	D3DXMatrixTranslation(&MyTransMat, MyPos.x, MyPos.y, MyPos.z);


	D3DXMatrixRotationY(&MyPRotMat, GetTickCount() * MyPAngleSpeed / 50000.0f);

	if (nullptr == MyParent)
	{
		MyFinalMat = MyRotMat * MyTransMat;
	}

	else
	{
		MyFinalMat = MyRotMat * MyTransMat * MyPRotMat * MyParent->MyFinalMat;
	}
}
