#pragma once
#include"BitMap.h"

enum IMAGENUM
{
	IMAGENUM_BLACKBACKGROUND,
	IMAGENUM_BACK,
	IMAGENUM_BACKDECO,
	IMAGENUM_BACKNORMAL,
	IMAGENUM_BACKNORMAL2,
	IMAGENUM_CASH,
	IMAGENUM_DIE,
	IMAGENUM_END,
	IMAGENUM_ENEMY,
	IMAGENUM_ENEMY_1b,
	IMAGENUM_ENEMY_1f,
	ENEMY_b,
	ENEMY_f,
	ENEMY_l,
	ENEMY_l_b,
	ENEMY_l_f,
	ENEMY1,
	FRONT,
	FRONT2,
	ICON,
	IMAGENUM_MITER,
	PLAYER0,
	PLAYER1,
	PLAYER2,
	STAR,
	STAR2,
	STAR3,
	WIN,
	WIN2
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