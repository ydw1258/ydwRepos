#include "ResManager.h"
#include "BitMap.h"
#include "resource.h"
#include "macro.h"

ResManager::~ResManager()
{
}
ResManager* ResManager::pResManager = nullptr;

void ResManager::Init(HDC hdc, HINSTANCE hInst, string _Path)
{
	BitMap* NewBit = new BitMap();
	NewBit->Init(hdc, hInst, _Path.c_str());
	m_pBitMap[_Path] = NewBit;
}

BitMap* ResManager::GetBitMap(string fileName)
{
	map<string, BitMap*>::iterator SIter = m_pBitMap.find(fileName);
	
	if (SIter != m_pBitMap.end())
	{
		//find
		return SIter->second;
	}
	return nullptr;
}

void ResManager::Release()
{
	map<string, BitMap*>::iterator SIter = m_pBitMap.begin();
	//map<string, BitMap*>::iterator EIter = m_pBitMap.end();

	for (SIter = m_pBitMap.begin(); SIter != m_pBitMap.end(); SIter++)
	{
		delete SIter->second;
	}

	m_pBitMap.clear();

	delete pResManager;
}