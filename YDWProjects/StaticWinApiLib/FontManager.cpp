#include "FontManager.h"
#include "ResourceManager.h"
#include<Windows.h>

void FontManager::Draw(HDC hdc, int num, int fontSize, int x, int y, string fontName, COLORREF RGB) //���� ���
{
	char outText[7];
	SetTextColor(hdc, RGB);
	//Font ����
	//���� ��� �����ϰ�
	//myFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(hdc, myFont);

	//Font ����
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

void FontManager::Draw(HDC hdc, string str, int fontSize, int x, int y, string fontName, COLORREF RGB)//���� ��� RGB(255, 255, 0) �������� �Է�
{
	//Font ����
	//���� ��� �����ϰ�
	myFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, fontName.c_str());
	oldFont = (HFONT)SelectObject(hdc, myFont);
	SetTextColor(hdc, RGB);
	//Font ����
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_TOP);
	TextOut(hdc, x, y, str.c_str(), strlen(str.c_str())); //strlen(szText)
	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}