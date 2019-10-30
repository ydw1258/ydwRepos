#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
using namespace std;

struct SRotAnimation
{
	int					nTime;
	D3DXQUATERNION		qRotate;
};

struct SPosAnimation
{
	int					nTime;
	D3DXVECTOR3			vPosition;
};

struct SFaceData
{
	int				nIndex[3];
	D3DXVECTOR3		vFaceNormal;
	D3DXVECTOR3		vNormal[3];
	int				nTIndex[3];

	SFaceData()
	{  
		nIndex[0] = nIndex[1] = nIndex[2] = -1;
		nTIndex[0] = nTIndex[1] = nTIndex[2] = -1;
		vFaceNormal = vNormal[0] = vNormal[1] = vNormal[2] = D3DXVECTOR3(-1000, -1000, -1000);
	}
};

struct SMeterial
{
	D3DMATERIAL9			dxMeterial;
	string					strTexture;
	bool					bSpecular;
	int						nSize;

	SMeterial()
	{
		memset(&dxMeterial, 0, sizeof(D3DMATERIAL9));
		strTexture = "";
		bSpecular = false;
		nSize = 0;
	}
};