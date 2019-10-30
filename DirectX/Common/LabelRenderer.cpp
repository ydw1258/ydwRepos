#include "LabelRenderer.h"
#include "SAFE_DELETE.h"
#include <assert.h>
#include <stdio.h>

LabelRenderer* LabelRenderer::m_this = nullptr;

LabelRenderer::LabelRenderer()
{
	m_pFont = nullptr;
}


LabelRenderer::~LabelRenderer()
{
}

void LabelRenderer::Init(LPDIRECT3DDEVICE9 device)
{
	HRESULT res = D3DXCreateFont(device, 20, 15, 1 , FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "±¼¸²", &m_pFont);

	if (FAILED(res))
	{
		MessageBox(NULL, "Font Load Failed!!" , "Error" , MB_OK);
		return;
	}
	
}

void LabelRenderer::DrawLabel(int nX, int nY, D3DXCOLOR color,const char* msg, ...)
{
	assert(m_pFont);

	char text[256];
	va_list vl;
	va_start(vl, msg);
	vsprintf(text, msg, vl);
	va_end(vl);

	RECT rc = { nX, nY, 0, 0 };
	m_pFont->DrawText(NULL, text, strlen(text), &rc, DT_NOCLIP, color);
}

void LabelRenderer::DrawLabel(const char* szText, int nX, int nY, D3DXCOLOR color)
{
	assert(m_pFont);

	RECT rc = { nX, nY, 0, 0 };
	m_pFont->DrawText(0, szText, strlen(szText), &rc, DT_NOCLIP, color);
}

void LabelRenderer::Release()
{
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_this);
}
