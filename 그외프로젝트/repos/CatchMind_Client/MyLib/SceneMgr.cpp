#include "SceneMgr.h"

SceneMgr* SceneMgr::pThis = nullptr;

SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
}

void SceneMgr::Init(HWND _hWnd, HINSTANCE _hInstance)
{
	hWnd = _hWnd;
	hInstance = _hInstance;
}

void SceneMgr::Input(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (curScene == nullptr)
		return;

	curScene->Input(iMessage, wParam, lParam);
}

void SceneMgr::ProcessPacket(char * szBuf, int len)
{
	if (curScene == nullptr)
		return;

	curScene->ProcessPacket(szBuf, len);
}

void SceneMgr::Render(HDC hdc)
{
	if (curScene == nullptr)
		return;

	curScene->Render(hdc);
}

void SceneMgr::Update(float deltaTime)
{
	if (curScene == nullptr)
		return;

	curScene->Update(deltaTime);
}

void SceneMgr::Release()
{
	for (auto iter = sceneList.begin(); iter != sceneList.end();)
	{
		(*iter).second->Release();
		delete (*iter).second;
		iter = sceneList.erase(iter);
	}

	if (pThis)
		delete pThis;
}

void SceneMgr::InsertScene(string key, Scene * scene)
{
	sceneList.insert(make_pair(key, scene));
}

void SceneMgr::ChangeScene(string sceneName)
{
	map<string, Scene*>::iterator iter;
	iter = sceneList.find(sceneName);

	if (iter == sceneList.end())
		return;

	if (curScene != nullptr)
		curScene->Release();

	curScene = (*iter).second;
	curScene->Init(hWnd, hInstance);
}



