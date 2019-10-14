#pragma once
#include<Windows.h>
#include"SpriteRenderer.h"

enum DIRECTION;

class Bullet
{
private:
	float speed;
	
	SpriteRenderer sprite;
public:
	DIRECTION direction;
	float x;
	float y;
	bool isPlayers;
	bool OutofRange();
	void Init(int PlayerX, int PlayerY, DIRECTION _direction);
	void Move(float deltaTime);
	void Draw(HDC hdc);
};