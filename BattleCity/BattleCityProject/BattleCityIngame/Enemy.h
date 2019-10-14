#pragma once
#include "SpriteRenderer.h"
#include "IMAGENUM.h"
#include "GameManager.h"
#include "Bullet.h"
#include <list>
using namespace std;

class Enemy
{
private:
	float speed = 100.0f;
	SpriteRenderer upSprite;
	SpriteRenderer downSprite;
	SpriteRenderer leftSprite;
	SpriteRenderer rightSprite;
	SpriteRenderer curSprite;
public:
	DIRECTION direction;
	float x;
	float y;
	
	void Update(float deltaTime);
	void Init(int _x, int _y);
	void Draw(HDC hdc);
	void Move(float deltaTime);
	void SpriteChange();
	bool ShotTimer(float deltaTime);
	void DirectionChange();
	int	sizeX = 32;
	int	sizeY = 32;
	Enemy();
	~Enemy();
};
