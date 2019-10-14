#include "Enemy.h"

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Update(float deltaTime)
{
	static float moveTimer = 2.0f;
	static float shotTimer = 1.0f;

	moveTimer -= deltaTime * 10;
	shotTimer -= deltaTime * 10;

	if (moveTimer < 0)
	{
		moveTimer = 4.0f;
		direction = (DIRECTION)(rand() % 4);
		//¿òÁ÷ÀÌ´Â ¹æÇâ º¯°æ
		
	}
	if (shotTimer < 0)
	{
		shotTimer = 1.0f;

		//ÃÑ½î±â
		int randNum = rand() % 3;
		if (randNum == 0)// 1/3È®·ü
		{
			Shot();
		}
	}
//	Move(deltaTime);
}

void Enemy::Init(int _x, int _y)
{
	x = _x;
	y = _y;
	sizeX = TILEBLOCK_SIZE;
	sizeY = TILEBLOCK_SIZE;
	upSprite.Init(IMAGENUM_ENEMY_UP01, 2, sizeX, sizeY);
	downSprite.Init(IMAGENUM_ENEMY_DOWN01, 2, sizeX, sizeY);
	leftSprite.Init(IMAGENUM_ENEMY_LEFT01, 2, sizeX, sizeY);
	rightSprite.Init(IMAGENUM_ENEMY_RIGHT01, 2, sizeX, sizeY);
	curSprite = upSprite;
	
	direction = DIRECTION::DOWN;
}

void Enemy::Draw(HDC hdc)
{
	curSprite.DrawObject(hdc, x, y);
}

void Enemy::Move(float deltaTime)
{
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

void Enemy::Shot()
{
	if (bullets.size() == 0)
	{
		Bullet bullet;
		bullet.isPlayers = false;
		bullet.Init(x, y, direction);
		bullets.push_back(bullet);
	}
}

void Enemy::SpriteChange()
{
	curSprite.SpriteChange();
}
