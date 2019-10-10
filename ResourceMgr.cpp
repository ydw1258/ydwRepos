#include "ResourceMgr.h"
#include "BitMap.h"
#include "Animator.h"

ResourceMgr* ResourceMgr::pThis = nullptr;

ResourceMgr::ResourceMgr()
{
}

ResourceMgr::~ResourceMgr()
{
}

void ResourceMgr::Init(HDC _hdc, HINSTANCE _hInstance)
{
	hdc = _hdc;
	hInstance = _hInstance;
}

void ResourceMgr::Release()
{
	for (auto iter = bitmap.begin(); iter != bitmap.end();)
	{
		(*iter).second->Release();
		delete (*iter).second;
		iter = bitmap.erase(iter);
	}

	if (pThis)
		delete pThis;
}

void ResourceMgr::InsertBitmap(string key, LPCWSTR path)
{
	BitMap* b = new BitMap();
	b->Init(hdc, hInstance, path); //¿ä±â
	bitmap.insert(make_pair(key, b));
}
