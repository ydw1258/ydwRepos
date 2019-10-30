#pragma once
#include <windows.h>
#include "d3dx9.h"
#include "d3d9.h"

class LabelRenderer
{
private:
	static LabelRenderer*	m_this;

	LPD3DXFONT				m_pFont;
public:
	static LabelRenderer* GetInstance()
	{
		if (m_this == nullptr)
		{
			m_this = new LabelRenderer();
		}

		return m_this;
	}

	void Init(LPDIRECT3DDEVICE9 device);
	void DrawLabel(int nX, int nY, D3DXCOLOR color, const char* msg, ...);
	void DrawLabel(const char* szText, int nX, int nY, D3DXCOLOR color);
	void Release();

	LabelRenderer();
	~LabelRenderer();
};

