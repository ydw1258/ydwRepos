#include "Sphere.h"

Sphere::Sphere(){}
Sphere::~Sphere(){}

bool Sphere::Init(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	//셰이더 로딩
	
	g_pColorShader = LoadShader("ColorShader.fx", g_pD3DDevice);
	if (!g_pColorShader)
		return false;

	g_pSphere = LoadModel("sphere.x", g_pD3DDevice);
	if (!g_pSphere)
		return false;
		

	//텍스처 지구 버전
	//셰이더 로딩
	/*
	g_pTextureShader = LoadShader("TextureMapping.fx" , g_pD3DDevice);
	if (!g_pTextureShader)
		return false;

	g_pSphere = LoadModel("sphere.x", g_pD3DDevice);
	if (!g_pSphere)
		return false;

	g_pEarthDM = LoadTexture("Earth.jpg", g_pD3DDevice);
	if (!g_pEarthDM)
		return false;
		*/
	return true;
}

LPD3DXEFFECT Sphere::LoadShader(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlag = 0;

#if _DEBUG
	dwShaderFlag |= D3DXSHADER_DEBUG;
#endif
	D3DXCreateEffectFromFile(g_pD3DDevice, fileName, NULL, NULL, dwShaderFlag, NULL, &ret, &pError);

	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

LPD3DXMESH Sphere::LoadModel(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, g_pD3DDevice,
		NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패 : ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}

	return ret;
}

LPDIRECT3DTEXTURE9 Sphere::LoadTexture(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, fileName, &ret)))
	{
		OutputDebugString("텍스쳐 로딩 실패 : ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}

void Sphere::Render()
{
	g_pSphere->DrawSubset(0);
}
