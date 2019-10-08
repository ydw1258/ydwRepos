#pragma once
#include "SpriteRenderer.h"

enum DIRECTION
{
	STOP,
	LEFT,
	RIGHT,
};
class Player
{
private:
	bool isJump = false;
	SpriteRenderer CurSprite;
	SpriteRenderer leftStop;
	SpriteRenderer rightStop;
	SpriteRenderer leftMove;
	SpriteRenderer rightMove;
	SpriteRenderer leftJump;
	SpriteRenderer rightJump;

public:
	float x;
	float y;
	void Init();
	void Move(DIRECTION dir);
	void Attack();
	void Jump(DIRECTION dir);
};


