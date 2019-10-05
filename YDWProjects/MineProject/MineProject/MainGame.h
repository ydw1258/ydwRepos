#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include"ResManager.h"
#include"MineButton.h"

using namespace std;

enum GAME_STATE
{
	GAME_PLAY,
	GAME_OVER
};

class MainGame
{
private:
	static MainGame* m_sThis;

	HWND			m_hWnd;
	GAME_STATE		m_eState;

	MainGame();
public:
	vector<MineButton*> vMineButton;

	static MainGame* GetInstance()
	{
		if (m_sThis == nullptr)
			m_sThis = new MainGame();

		return m_sThis;
	}

	void Init(HWND hWnd, HDC hdc, HINSTANCE hInst);
	void Draw(HDC hdc);
	void Update();
	void Input(POINT pt);
	void Release();

	MineButton* GetButton(int idx)
	{
		return vMineButton[idx];
	}

	~MainGame();
};