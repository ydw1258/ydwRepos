#include "Player.h"
#include "GameManager.h"

Player::Player() {}
Player::~Player() {}

void Player::Init(int _sizeX, int _sizeY, IMAGENUM imageNum)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	upSprite.Init(IMAGENUM_ROW_1_UP_00, 2, sizeX, _sizeY);
	downSprite.Init(IMAGENUM_ROW_1_DOWN_00, 2, sizeX, _sizeY);
	leftSprite.Init(IMAGENUM_ROW_1_LEFT_00, 2, sizeX, _sizeY);
	rightSprite.Init(IMAGENUM_ROW_1_RIGHT_00, 2, sizeX, _sizeY);
	curSprite = upSprite;
}

void Player::PlayerMove(DIRECTION direction)
{
	switch (direction)
	{
	case UP:
		curSprite = upSprite;
		if (y > GameManager::GetInstance()->GameOffsetY)
			y-= speed;
		break;
	case DOWN:
		curSprite = downSprite;
		if (y < sizeY * 12 + GameManager::GetInstance()->GameOffsetY)
			y+= speed;
		break;
	case LEFT:
		curSprite = leftSprite;
		if (x > GameManager::GetInstance()->GameOffsetX)
			x -= speed;
		break;
	case RIGHT:
		curSprite = rightSprite;
		if (x < sizeX * 12 + GameManager::GetInstance()->GameOffsetX)
			x+= speed;
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