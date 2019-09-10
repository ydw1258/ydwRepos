#include "Player.h"
#include "GameManager.h"

Player::Player(){}
Player::~Player(){}

//EditMode
void Player::EditDraw(HDC hdc, int offsetX, int offsetY)
{
	upSprite.DrawObject(hdc, x * sizeX + offsetX, y * sizeY + offsetY);
}

void Player::Init(int _sizeX, int _sizeY, IMAGENUM imageNum)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
}

void Player::PlayerMove(DIRECTION direction)
{
	switch (direction)
	{
	case UP:
		if (y > 0)
			y--;
		break;
	case DOWN:
		if (y < TILE_HEIGHT_NUM - 1)
			y++;
		break;
	case LEFT:
		if (x > TILE_WIDTH_NUM - 1)
			x--;
		break;
	case RIGHT:
		if (x < 12)
			x++;
		else
			return;
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
