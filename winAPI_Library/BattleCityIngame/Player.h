#pragma once
#include"SpriteRenderer.h"

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
	float speed = 0.4f;
	SpriteRenderer upSprite;
	SpriteRenderer downSprite;
	SpriteRenderer leftSprite;
	SpriteRenderer rightSprite;
	int	sizeX;
	int	sizeY;
	DIRECTION direction;
	SpriteRenderer curSprite;
public:
	float x = 0;
	float y = 0;
	Player();
	~Player();

	void Init(int _sizeX, int _sizeY, IMAGENUM imageNum);
	void Draw(HDC hdc, int offsetX, int offsetY);
	void PlayerMove(DIRECTION direction);
	void SpriteChange();
};

