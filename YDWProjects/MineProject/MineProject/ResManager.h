#pragma once
#include <Windows.h>
#include<map>
#include<string>
#define RESOURCENUM 20
using namespace std;

class BitMap;
class ResManager
{
private:
	ResManager() {};
	~ResManager();

public:
	static ResManager* inst()
	{
		if (nullptr == pResManager)
		{
			pResManager = new ResManager();
		}

		return pResManager;
	}

private:
	static ResManager* pResManager;
	map<string, BitMap*> m_pBitMap;


public:
	void Init(HDC hdc, HINSTANCE hInst, string _Path);
	BitMap* GetBitMap(string fileName);
	void Release();
};

