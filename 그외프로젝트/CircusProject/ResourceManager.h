#pragma once
#include"BitMap.h"

enum IMAGENUM
{
	IMAGENUM_BLACKBACKGROUND, 	IMAGENUM_BACK,	IMAGENUM_BACKDECO,
	IMAGENUM_BACKNORMAL,	IMAGENUM_BACKNORMAL2,	IMAGENUM_CASH,
	IMAGENUM_DIE,	IMAGENUM_END,	IMAGENUM_ENEMY,
	IMAGENUM_ENEMY_1b,	IMAGENUM_ENEMY_1f,	IMAGENUM_ENEMY_b,
	IMAGENUM_ENEMY_f,	IMAGENUM_ENEMY_l,	IMAGENUM_ENEMY_l_b,
	IMAGENUM_ENEMY_l_f,	IMAGENUM_ENEMY1,	FRONT,
	FRONT2,	ICON,	IMAGENUM_MITER,
	PLAYER0,	PLAYER1,	PLAYER2,
	IMAGENUM_STAR,	IMAGENUM_STAR2,	IMAGENUM_STAR3,
	IMAGENUM_WIN,	IMAGENUM_WIN2
};

class ResourceManager
{
private:
	static ResourceManager* mthis;
	BitMap* bitmap;
public:
	static BitMap* backBuffer;

	static ResourceManager* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new ResourceManager();
		return mthis;
	}
	
	void Release();
	void Init(HDC hdc, HINSTANCE hInstance);
	void Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum);
	ResourceManager();
	~ResourceManager();
};