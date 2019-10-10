#include "FontManager.h"
#include "ResourceManager.h"
#include<Windows.h>

void FontManager::Draw(HDC hdc, int num, int fontSize, int x, int y, string fontName, COLORREF RGB) //숫자 출력
{
	char outText[7];
	SetTextColor(hdc, RGB);
	//Font 설정
	//글자 배경 투명하게
	//myFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(hdc, myFont);

	//Font 설정
	SetBkMode(hdc, TRANSPARENT);

	wsprintf(outText, "%d", num);
	TextOut(hdc, x, y, outText, strlen(outText)); //strlen(szText)

	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}

void FontManager::Init()
{
	AddFontResourceA("Resource\DungGeunMo.ttf");
}

void FontManager::Draw(HDC hdc, string str, int fontSize, int x, int y, string fontName, COLORREF RGB)//문자 출력 RGB(255, 255, 0) 형식으로 입력
{
	//Font 설정
	//글자 배경 투명하게
	myFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(hdc, myFont);
	SetTextColor(hdc, RGB);
	//Font 설정
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_TOP);
	TextOut(hdc, x, y, str.c_str(), strlen(str.c_str())); //strlen(szText)
	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}