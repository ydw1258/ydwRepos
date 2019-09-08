#pragma once
#include"ResourceManager.h"
#include"SpriteRenderer.h"

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
	bool isJump = false;
	bool isUp = true;
	int jumpTopY = 550;
	SpriteRenderer moveSprite[2];
	SpriteRenderer jumpSprite;
	SpriteRenderer dieSprite;
	IMAGENUM curSprite = PLAYER0;
public:
	RECT GetPlayerRect();
	void Init();
	void SpriteChange();
	PLAYERSTATE playerState = STOP;
	void Move(bool direction);
	void Jump();
	void Draw(HDC hdc);

	void SetJump()
	{
		isJump = true;
	}
	void SetDie()
	{
		playerState = PLAYER_DIE;
		curSprite = IMAGENUM_DIE;
	}

	void Stop();
	Player();
	~Player();
};