#include "MainGame.h"
#include "SceneMgr.h"

#include <iostream>
#include "MyUtil.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Init(HWND hWnd, HDC hdc, HINSTANCE hInstance)
{
	SceneMgr::GetInstance()->Init(hWnd,hdc,hInstance);
}

void MainGame::Update()
{
	SceneMgr::GetInstance()->Update();
}

void MainGame::Input(WPARAM wParam, LPARAM lParam, int btnIdx, bool up)
{
	SceneMgr::GetInstance()->Input(wParam, lParam, btnIdx, up);
}

void MainGame::Render()
{
	SceneMgr::GetInstance()->Render();
}

void MainGame::Release()
{

}
