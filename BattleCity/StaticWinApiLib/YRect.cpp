#include "YRect.h"

YRect::YRect(RECT* rect)
{
	myRect.top = rect->top;
	myRect.bottom = rect->bottom;
	myRect.left = rect->left;
	myRect.right = rect->right;
}

YRect::YRect(RECT& rect)
{
	myRect.top = rect.top;
	myRect.bottom = rect.bottom;
	myRect.left = rect.left;
	myRect.right = rect.right;
}

YRect::~YRect()
{
}

void YRect::DrawRect(HDC hdc, int x, int y, int cx, int cy, int imageNum, RGBInfo rgb)
{
	HBRUSH tempBrush, tempOBrush;
	
	tempBrush = CreateSolidBrush(RGB(rgb.r, rgb.g, rgb.b));//색깔
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);

	Rectangle(hdc, myRect.left, myRect.top, myRect.right, myRect.bottom);
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);

	
	//채우기는 투명하게

}
void YRect::DrawTransparentRect(HDC hdc, int x, int y, int cx, int cy, int imageNum, RGBInfo rgb)
{
	HBRUSH tempBrush, tempOBrush;

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);

	Rectangle(hdc, myRect.left, myRect.top, myRect.right, myRect.bottom);
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);

	HPEN pen, oPen;
	PAINTSTRUCT ps;

	pen = CreateSolidBrush(RGB(rgb.r, rgb.g, rgb.b));


}