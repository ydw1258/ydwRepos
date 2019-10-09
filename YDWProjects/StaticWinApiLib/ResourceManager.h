#pragma once
#include <Windows.h>
#include<map>
#include<string>
#include"BitMap.h"
using namespace std;

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
	void Init(HDC hdc, string* fileNames, int fileNameCount);
	void Draw(HDC hdc, int x, int y, int cx, int cy, int imageNum);
	ResourceManager();
	~ResourceManager();
};