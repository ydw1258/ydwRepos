#include "FontManager.h"

void FontManager::Init()
{
	//sprite.Init(IMAGENUM_MITER, 1, 86, 30);
	//wsprintf(outText, "%d", GameManager::GetIntance()->distance);
	
	// 매개변수 DC의 글자배경을 투명하게 한다
	//SetBkColor(hdc, RGB(255, 255, 0));
}

void FontManager::Draw(int num, int x, int y)
{
	char outText[3];
	SetTextColor(ResourceManager::backBuffer->GetmemDC(), RGB(255, 255, 0));

	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "godoMaum");
	oldFont = (HFONT)SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);

	//Font 설정
	SetBkMode(ResourceManager::backBuffer->GetmemDC(), TRANSPARENT);
	wsprintf(outText, "%d", num);
	TextOut(ResourceManager::backBuffer->GetmemDC(), x + 150, y, outText, 2); //strlen(szText)
}
void FontManager::moveDraw(int num, int x, int y, int scrollSpeedX, int scrollSpeedY, int resetX, int resetY)
{
	char outText[3];
	SetTextColor(ResourceManager::backBuffer->GetmemDC(), RGB(255, 255, 0));
	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "godoMaum");
	oldFont = (HFONT)SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);

	//Font 설정
	SetBkMode(ResourceManager::backBuffer->GetmemDC(), TRANSPARENT);
	
	wsprintf(outText, "%d", num);
	TextOut(ResourceManager::backBuffer->GetmemDC(), x - fontOffsetX + 30, y - fontOffsetY, outText, 2); //strlen(szText)
	fontOffsetX -= scrollSpeedX;
	fontOffsetY -= scrollSpeedY;

	//수정 예정
	if (x - fontOffsetX - resetX < 0)
		fontOffsetX = 0;
	if (x - fontOffsetY - resetY < 0)
		fontOffsetY = 0;
}
void FontManager::fontOffsetResetX()
{
	fontOffsetX = 0;
}
void FontManager::fontOffsetResetY()
{
	fontOffsetY = 0;
}
void FontManager::Release()
{
	SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);
	DeleteObject(myFont);
}

void FontManager::Draw(const char* str, int x, int y)
{
	char outText[3];
	SetTextColor(ResourceManager::backBuffer->GetmemDC(), RGB(255, 255, 0));

	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "godoMaum");
	oldFont = (HFONT)SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);

	//Font 설정
	SetBkMode(ResourceManager::backBuffer->GetmemDC(), TRANSPARENT);
	TextOut(ResourceManager::backBuffer->GetmemDC(), x + 150, y, outText, 10); //strlen(szText)
}
