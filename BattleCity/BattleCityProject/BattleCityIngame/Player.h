#pragma once
#include"SpriteRenderer.h"
#include "IMAGENUM.h"
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class Player
{
private:
	float speed = 100.0f;
	SpriteRenderer upSprite;
	SpriteRenderer downSprite;
	SpriteRenderer leftSprite;
	SpriteRenderer rightSprite;
	int	sizeX;
	int	sizeY;
	
	SpriteRenderer curSprite;
public:
	DIRECTION direction;
	float x = 0;
	float y = 0;
	Player();
	~Player();

	void Init();
	void Draw(HDC hdc);
	void PlayerMove(DIRECTION direction, float deltaTime);
	void SpriteChange();
};