#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

class BitMap;
class Animator;
class ResourceMgr
{
private:
	static ResourceMgr* pThis;

public:
	static ResourceMgr* GetInstance()
	{
		if (pThis == nullptr)
		{

			pThis = new ResourceMgr();
		}

		return pThis;
	}

private:
	HDC hdc;
	HINSTANCE hInstance;

	map<string, BitMap*> bitmap;

public:
	ResourceMgr();
	~ResourceMgr();

	void Init(HDC hdc, HINSTANCE hInstance);
	void Release();

	void InsertBitmap(string key, LPCWSTR path);
	BitMap* GetBitmap(string key) { return bitmap[key]; }
};

