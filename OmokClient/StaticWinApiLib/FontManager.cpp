#include "FontManager.h"
#include "ResourceManager.h"
#include<Windows.h>


//sprite.Init(IMAGENUM_MITER, 1, 86, 30);
//wsprintf(outText, "%d", GameManager::GetIntance()->distance);

// 매개변수 DC의 글자배경을 투명하게 한다
//SetBkColor(hdc, RGB(255, 255, 0));

void FontManager::Draw(int num, int x, int y, string fontName, COLORREF RGB) //숫자 출력
{

	char outText[7];
	SetTextColor(ResourceManager::backBuffer->GetmemDC(), RGB);
	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);

	//Font 설정
	SetBkMode(ResourceManager::backBuffer->GetmemDC(), TRANSPARENT);

	wsprintf(outText, "%d", num);
	TextOut(ResourceManager::backBuffer->GetmemDC(), x, y, outText, strlen(outText)); //strlen(szText)
}
void FontManager::Draw(string str, int x, int y, string fontName, COLORREF RGB)//문자 출력 RGB(255, 255, 0) 형식으로 입력
{
	SetTextColor(ResourceManager::backBuffer->GetmemDC(), RGB);

	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);

	//Font 설정
	SetBkMode(ResourceManager::backBuffer->GetmemDC(), TRANSPARENT);
	TextOut(ResourceManager::backBuffer->GetmemDC(), x, y, str.c_str(), strlen(str.c_str())); //strlen(szText)
}

void FontManager::Release()
{
	SelectObject(ResourceManager::backBuffer->GetmemDC(), myFont);
	DeleteObject(myFont);
}
