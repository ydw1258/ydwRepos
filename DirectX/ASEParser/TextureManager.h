#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include <map>
using namespace std;

class TextureManager
{
private:
	static TextureManager* m_pThis;

	map<string, LPDIRECT3DTEXTURE9>		m_smTexture;
public:
	static TextureManager* GetInstance()
	{
		if (m_pThis == nullptr)
		{
			m_pThis = new TextureManager();
		}

		return m_pThis;
	}

	LPDIRECT3DTEXTURE9 GetTexture(LPDIRECT3DDEVICE9 pDevice, const char* szName);

	TextureManager();
	~TextureManager();
};

