#pragma once
#include<Windows.h>
#include"SpriteRenderer.h"

enum DIRECTION;

class Bullet
{
private:
	float x;
	float y;
	float speed = 1.2f;
	DIRECTION direction;
	SpriteRenderer sprite;
public:
	bool isPlayers;
	bool OutofRange();
	void Init(int PlayerX, int PlayerY, DIRECTION _direction);
	void Move();
	void Draw(HDC hdc);
};