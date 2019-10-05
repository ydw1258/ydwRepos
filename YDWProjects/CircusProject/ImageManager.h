#pragma once
#include<Windows.h>

enum IMAGENUM
{
	BLACKBACKGROUND,
	BACK,
	BACKDECO,
	BACKNORMAL,
	BACKNORMAL2,
	CASH,
	DIE,
	END,
	ENEMY,
	ENEMY_1b,
	ENEMY_1f,
	ENEMY_b,
	ENEMY_f,
	ENEMY_l,
	ENEMY_l_b,
	ENEMY_l_f,
	ENEMY1,
	FRONT,
	FRONT2,
	ICON,
	MITER,
	PLAYER0,
	PLAYER1,
	PLAYER2,
	STAR,
	STAR2,
	STAR3,
	WIN,
	WIN2
};

class ImageManager
{
private:
	static ImageManager* mthis;
	HDC memDC[50];
	HBITMAP hBitmap[50];
	HBITMAP old[50];
public:
	static ImageManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ImageManager();
		return mthis;
	}

	void Init(HDC hdc);
	void Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum);
	ImageManager();
	~ImageManager();
};
