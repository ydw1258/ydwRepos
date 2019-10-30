#include "TextureManager.h"

TextureManager* TextureManager::m_pThis = nullptr;

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(LPDIRECT3DDEVICE9 pDevice , const char* textureName)
{
	map<string, LPDIRECT3DTEXTURE9>::iterator itor = m_smTexture.find(textureName);

	if (itor != m_smTexture.end())
		return itor->second;

	LPDIRECT3DTEXTURE9 pTexture;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, textureName, &pTexture)))
	{
		return 0;
	}

	if (pTexture != 0)
		m_smTexture.insert(make_pair(textureName, pTexture));

	return pTexture;
}
