#pragma once
#include <Windows.h>
#include <map>
#include "Scene.h"

using namespace std;

class SceneMgr
{
private:
	static SceneMgr* pThis;

public:
	static SceneMgr* GetInstance()
	{
		if (pThis == nullptr)
		{
			pThis = new SceneMgr();
		}

		return pThis;
	}

private:
	map<string, Scene*> sceneList;
	Scene* curScene = nullptr;

	HWND hWnd;
	HINSTANCE hInstance;

public:
	SceneMgr();
	~SceneMgr();

	void Init(HWND hWnd, HINSTANCE hInstance);
	void Input(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(char * szBuf, int len);
	void Render(HDC hdc);
	void Update(float deltaTime);
	void Release();

	void InsertScene(string key, Scene* scene);
	void ChangeScene(string sceneName);
};
