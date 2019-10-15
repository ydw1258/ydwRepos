#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

int Scene::Init(HWND _hWnd, HINSTANCE _hInstance)
{
	hWnd = _hWnd;
	hInstance = _hInstance;

	isInit = true;

	return 0;
}

void Scene::Input()
{

}

void Scene::Input(UINT iMessage, WPARAM wParam, LPARAM lParam)
{

}

void Scene::Render(HDC hdc)
{

}

void Scene::Update(float deltatime)
{

}

void Scene::ProcessPacket(char * szBuf, int len)
{

}

void Scene::Release()
{

}
