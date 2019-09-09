#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
}
//EditMode
void Player::EditDraw(HDC hdc, int offsetX, int offsetY)
{
	upSprite.DrawObject(hdc, x, y);
}

void Player::Init(int _sizeX, int _sizeY, IMAGENUM imageNum)
{
	upSprite.Init(IMAGE_NUM_PLAYERTANK_UP_00, 2, 33, 26);
	downSprite.Init(IMAGE_NUM_PLAYERTANK_DOWN_00, 2, 33, 26);
	leftSprite.Init(IMAGE_NUM_PLAYERTANK_LEFT_00, 2, 33, 26);
	rightSprite.Init(IMAGE_NUM_PLAYERTANK_RIGHT_00, 2, 33, 26);
}

void Player::PlayerMoveInEditMode(DIRECTION direction)
{
	switch (direction)
	{
	case UP:
		if (y > 0)
			y -= sizeY;
		break;
	case DOWN:
		if (x < 13 * sizeX)
			x -= sizeX;
		break;
	case LEFT:
		if (x > 0)
			x -= sizeX;
		break;
	case RIGHT:
		if (y < 13 * sizeY)
			y -= sizeY;
		break;
	default:
		break;
	}
}

//Ingame
void Player::Draw(HDC hdc, int offsetX, int offsetY)
{
	switch (direction)
	{
	case UP:
		break;
	case DOWN:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	default:
		break;
	}
}
