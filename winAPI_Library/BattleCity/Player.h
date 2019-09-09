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
	SpriteRenderer upSprite;
	SpriteRenderer downSprite;
	SpriteRenderer leftSprite;
	SpriteRenderer rightSprite;
	int x = 0;
	int y = 0;
	int	sizeX;
	int	sizeY;
	DIRECTION direction;
	SpriteRenderer curSprite;
public:
	Player();
	~Player();
	//InGame
	void EditDraw(HDC hdc, int offsetX, int offsetY);
	//InEdit
	void Init(int _sizeX, int _sizeY, IMAGENUM imageNum);
	void Draw(HDC hdc, int offsetX, int offsetY);
	void PlayerMoveInEditMode(DIRECTION direction);
};

