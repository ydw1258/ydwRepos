#include "Player.h"
#include "GameManager.h"

Player::Player() {}
Player::~Player() {}

void Player::Init()
{
	sizeX = 32;
	sizeY = 32;
	//초기 위치 4, 13 집은 6, 13
	x = 4 * TILEBLOCK_SIZE + GameManager::GetInstance()->GameOffsetX;
	y = 4 * TILEBLOCK_SIZE + GameManager::GetInstance()->GameOffsetY;

	upSprite.Init(IMAGENUM_PLAYER_UP01, 2, sizeX, sizeY);
	downSprite.Init(IMAGENUM_PLAYER_DOWN01, 2, sizeX, sizeY);
	leftSprite.Init(IMAGENUM_PLAYER_LEFT01, 2, sizeX, sizeY);
	rightSprite.Init(IMAGENUM_PLAYER_RIGHT01, 2, sizeX, sizeY);
	curSprite = upSprite;
}

void Player::PlayerMove(DIRECTION _direction, float deltaTime)
{
	direction = _direction;
	switch (direction)
	{
	case UP:
		curSprite = upSprite;
		if (y > GameManager::GetInstance()->GameOffsetY)
			y-= deltaTime * speed;
		break;
	case DOWN:
		curSprite = downSprite;
		if (y < sizeY * 12 + GameManager::GetInstance()->GameOffsetY)
			y+= deltaTime * speed;
		break;
	case LEFT:
		curSprite = leftSprite;
		if (x > GameManager::GetInstance()->GameOffsetX)
			x -= deltaTime * speed;
		break;
	case RIGHT:
		curSprite = rightSprite;
		if (x < sizeX * 12 + GameManager::GetInstance()->GameOffsetX)
			x += deltaTime * speed;
		break;
	}
}

void Player::Draw(HDC hdc)
{
	curSprite.DrawObject(hdc, x, y);
}
void Player::SpriteChange()
{
	curSprite.SpriteChange();
}
