#include "Player.h"
#include "GameManager.h"

Player::Player() {}
Player::~Player() {}

void Player::Init()
{
	//player초기 위치 4, 12
	x = TILEBLOCK_SIZE * 4 + GameManager::GetInstance()->GameOffsetX;
	y = TILEBLOCK_SIZE * 12 + GameManager::GetInstance()->GameOffsetY;
	sizeX = TILEBLOCK_SIZE;
	sizeY = TILEBLOCK_SIZE;
	upSprite.Init(IMAGENUM_PLAYER_UP01, 2, sizeX, sizeY);
	downSprite.Init(IMAGENUM_PLAYER_DOWN01, 2, sizeX, sizeY);
	leftSprite.Init(IMAGENUM_PLAYER_LEFT01, 2, sizeX, sizeY);
	rightSprite.Init(IMAGENUM_PLAYER_RIGHT01, 2, sizeX, sizeY);
	curSprite = upSprite;
}

void Player::Move(DIRECTION _direction, float deltaTime)
{
	direction = _direction;

	switch (direction)
	{
	case UP:
		curSprite = upSprite;
		if (y > GameManager::GetInstance()->GameOffsetY)
			y -= deltaTime * speed;
		break;
	case DOWN:
		curSprite = downSprite;
		if (y < sizeY * 12 + GameManager::GetInstance()->GameOffsetY)
			y += deltaTime * speed;
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
