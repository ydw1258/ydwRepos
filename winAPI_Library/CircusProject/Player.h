#pragma once
#include"ResourceManager.h"

enum PLAYERSTATE
{
	STOP,
	LEFT,
	RIGHT,
	PLAYER_DIE
};
class Player
{
	int defaultY = 650;
	int x = 100;
	int y = defaultY;
	int jumpspeed = 2;
	bool isJump;
	bool isUp = true;
	int jumpTopY = 550;
	PLAYERSTATE playerState = STOP;
	IMAGENUM playerImage = PLAYER0;
public:
	RECT playerRECT;
	void Move(bool direction);
	void Jump();
	void Draw(HDC hdc);
	void SetJump()
	{
		isJump = true;
		playerImage = PLAYER2;
	}
	PLAYERSTATE GetPlayerState()
	{
		return playerState;
	}
	void SetDie()
	{
		playerState = PLAYER_DIE;
		playerImage = DIE;
	}

	void Stop();
	Player();
	~Player();
	void RECTDisplay(HDC hdc);
};