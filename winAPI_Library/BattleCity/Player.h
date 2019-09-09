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
public:
	Player();
	~Player();
#pragma once
	SpriteRenderer upSprite;
	SpriteRenderer downSprite;
	SpriteRenderer leftSprite;
	SpriteRenderer rightSprite;
	int x;
	int y;
	int	sizeX;
	int	sizeY;
	DIRECTION direction;
	SpriteRenderer curSprite;
public:
	//InGame
	void EditDraw(HDC hdc, int offsetX, int offsetY);
	//InEdit
	void Init(int _sizeX, int _sizeY, IMAGENUM imageNum);
	void Draw(HDC hdc, int offsetX, int offsetY);
	void PlayerMoveInEditMode(DIRECTION direction);
};

