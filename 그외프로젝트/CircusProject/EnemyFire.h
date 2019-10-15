#pragma once
#include"SpriteRenderer.h"
#include "Player.h"
enum FIRETYPE
{
	NORMAL,
	SMALL,
	FUTON
};
class EnemyFire
{
private:
	int moveSpeed;
	int x;
	int y;
	SpriteRenderer* spriteRenderer;
	FIRETYPE fireType;
public:
	EnemyFire();
	~EnemyFire();
	void Init(FIRETYPE _fireType, int _x, int _y, int _moveSpeed);
	void Move(int _moveSpeed);
	bool OutOfScreen();
	void Draw(HDC hdc, PLAYERSTATE playerState);
	RECT GetFireRect();
};