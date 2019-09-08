#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	RECT rc;
	
	bitmap = new BitMap[30];
	backBuffer = bitmap[0].Create(hdc, 1024, 720);

	bitmap[1].Init(hdc, hInstance, "back.bmp");
	bitmap[2].Init(hdc, hInstance, "back_deco.bmp");
	bitmap[3].Init(hdc, hInstance, "back_normal.bmp");
	bitmap[4].Init(hdc, hInstance, "back_normal2.bmp");
	bitmap[5].Init(hdc, hInstance, "cash.bmp");
	bitmap[6].Init(hdc, hInstance, "die.bmp");
	bitmap[7].Init(hdc, hInstance, "end.bmp");
	bitmap[8].Init(hdc, hInstance, "enemy.bmp");
	bitmap[9].Init(hdc, hInstance, "enemy_1b.bmp");
	bitmap[10].Init(hdc, hInstance, "enemy_1f.bmp");
	bitmap[11].Init(hdc, hInstance, "enemy_b.bmp");
	bitmap[12].Init(hdc, hInstance, "enemy_f.bmp");
	bitmap[13].Init(hdc, hInstance, "enemy_l.bmp");
	bitmap[14].Init(hdc, hInstance, "enemy_l_b.bmp");
	bitmap[15].Init(hdc, hInstance, "enemy_l_f.bmp");
	bitmap[16].Init(hdc, hInstance, "enemy1.bmp");
	bitmap[17].Init(hdc, hInstance, "front.bmp");
	bitmap[18].Init(hdc, hInstance, "front2.bmp");
	bitmap[19].Init(hdc, hInstance, "icon.bmp");
	bitmap[20].Init(hdc, hInstance, "miter.bmp");
	bitmap[21].Init(hdc, hInstance, "player0.bmp");
	bitmap[22].Init(hdc, hInstance, "player1.bmp");
	bitmap[23].Init(hdc, hInstance, "player2.bmp");
	bitmap[24].Init(hdc, hInstance, "star.bmp");
	bitmap[25].Init(hdc, hInstance, "star2.bmp");
	bitmap[26].Init(hdc, hInstance, "star3.bmp");
	bitmap[27].Init(hdc, hInstance, "win.bmp");
	bitmap[28].Init(hdc, hInstance, "win2.bmp");

	AddFontResourceA("godoMaum.ttf");
	
	// 2. 정보 설정하기 
	/*
	LOGFONT lf; 
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = -MulDiv(24, dc->GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = FW_NORMAL; lf.lfOutPrecision = OUT_TT_ONLY_PRECIS; strcpy(lf.lfFaceName, ("Gesso"));
	// 3. 폰트 만들기
	CFont newFont;
	if(!newFont.CreateFontIndirect(&lf))
		return; // 4. 사용할 폰트 선택하기
	CFont *pFont = dc->SelectObject(&newFont);
	// 5. 쓰고 폰트 이전껄로 돌리고 만든 폰트 지우기. 
	dc->TextOut(50, 40, "slway", 5); dc->SelectObject(pFont); newFont.DeleteObject();
	*/
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Release()
{
	delete[] bitmap;
	delete mthis;
}