#include "Player.h"
#include "GameManager.h"

Player::Player() {}
Player::~Player() {}

void Player::Init(int _sizeX, int _sizeY, IMAGENUM imageNum)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	upSprite.Init(IMAGENUM_ROW_1_UP_00, 2, 64, 64);
	downSprite.Init(IMAGENUM_ROW_1_DOWN_00, 2, 64, 64);
	leftSprite.Init(IMAGENUM_ROW_1_LEFT_00, 2, 64, 64);
	rightSprite.Init(IMAGENUM_ROW_1_RIGHT_00, 2, 64, 64);
	curSprite = upSprite;
}

void Player::PlayerMove(DIRECTION direction)
{
	switch (direction)
	{
	case UP:
		curSprite = upSprite;
		if (y > 0)
			y--;
		break;
	case DOWN:
		curSprite = downSprite;
		if (y < 12)
			y++;
		break;
	case LEFT:
		curSprite = leftSprite;
		if (x > 0)
			x--;
		break;
	case RIGHT:
		curSprite = rightSprite;
		if (x < 12)
			x++;
		break;
	}
}

void Player::Draw(HDC hdc, int offsetX, int offsetY)
{
	curSprite.DrawObject(hdc, x, y);
}
void Player::SpriteChange()
{
	curSprite.SpriteChange();
}