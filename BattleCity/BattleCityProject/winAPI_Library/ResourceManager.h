#pragma once
#include <Windows.h>
#include<map>
#include<string>
#include"BitMap.h"
using namespace std;

enum IMAGENUM
{
	IMAGENUM_BLACKBACKGROUND,
	IMAGENUM_BACK,
	IMAGENUM_BACKDECO,
	IMAGENUM_PLAYER_1, //PLAYER이미지가 3장있다고 가정
	IMAGENUM_PLAYER_2,
	IMAGENUM_PLAYER_3

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