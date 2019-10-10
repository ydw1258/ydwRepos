#include "UIButton.h"

void UIButton::Init(int buttonImageNum, int _x, int _y, int _sizeX, int _sizeY, char* _label)
{
	x = _x;
	y = _y;
	sizeX = _sizeX;
	sizeY = _sizeY;
	strcpy(label, _label);
	sprite.Init(buttonImageNum, 1, sizeX, sizeY);
}

void UIButton::Draw(HDC hdc, char* fontdir)
{
	sprite.DrawObject(hdc, x, y);
	//default 가운데 정렬
	font.Draw(hdc, label, 10, x, y, fontdir, RGB(0, 0, 0));
}

void UIButton::DrawRect(HDC hdc)
{
	Rectangle(hdc, x, y, x + sizeX, y + sizeY);
}

bool UIButton::isButtonClick(POINT pt)
{
	RECT rect;
	rect.left = x;
	rect.right = x + sizeX;
	rect.top = y;
	rect.bottom = y + sizeY;
	if (Physics::GetInstance()->RECTbyPointCollisionCheck(rect, pt))
		return true;
	else
		return false;
}
