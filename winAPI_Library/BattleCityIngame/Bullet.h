#pragma once
#include<Windows.h>
#include"SpriteRenderer.h"

enum DIRECTION;

class Bullet
{
private:
	bool isPlayers;
	
	float speed = 1.2f;
	DIRECTION direction;
	SpriteRenderer sprite;
public:
	const int BULLETSIZEXY = 6;
	float x;
	float y;
	bool OutofRange();
	void Init(int PlayerX, int PlayerY, DIRECTION _direction);
	void Move();
	void Draw(HDC hdc);
	Bullet() {}
	~Bullet() {}
};