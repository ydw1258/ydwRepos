#include "EnemyFire.h"
#include "ResourceManager.h"
#include "GameManager.h"

EnemyFire::EnemyFire(){}
EnemyFire::~EnemyFire(){}

void EnemyFire::Init(FIRETYPE _fireType, int _x, int _y, int _moveSpeed)
{
	x = _x + GameManager::GetInstance()->CameraX;
	y = _y;
	moveSpeed = _moveSpeed;
	fireType = _fireType;
	
	switch (fireType)
	{
	case NORMAL:
		spriteRenderer = new SpriteRenderer[2];
		spriteRenderer[0].Init(IMAGENUM_ENEMY_1b, 2, 25, 130);
		spriteRenderer[1].Init(IMAGENUM_ENEMY_1f, 2, 25, 130);
		break;
	case SMALL:
		spriteRenderer = new SpriteRenderer[2];
		spriteRenderer[0].Init(IMAGENUM_ENEMY_l_b, 2, 26, 104);
		spriteRenderer[1].Init(IMAGENUM_ENEMY_l_f, 2, 25, 104);
		break;
	case FUTON:
		spriteRenderer = new SpriteRenderer();
		spriteRenderer->Init(FRONT, 2, 50, 50);
		moveSpeed = 0;
		break;
	default:
		break;
	}
}
void EnemyFire::Draw(HDC hdc, PLAYERSTATE playerState)
{
	switch (fireType)
	{
	case NORMAL:
		spriteRenderer[0].DrawObject(hdc, x , y);
		spriteRenderer[1].DrawObject(hdc, x+ 25, y);
		break;
	case SMALL:
		spriteRenderer[0].DrawObject(hdc, x, y);
		spriteRenderer[1].DrawObject(hdc, x+26, y);

		break;
	case FUTON:
		spriteRenderer[0].DrawObject(hdc, x, y);
		break;
	}
}

RECT EnemyFire::GetFireRect()
{
	RECT fireRect;

	switch (fireType)
	{
	case NORMAL:
		fireRect.left = x + 15 - GameManager::GetInstance()->CameraX;
		fireRect.top = y + 120;
		fireRect.right = fireRect.left + 20;
		fireRect.bottom = fireRect.top + 20;
		break;
	case SMALL:
		fireRect.left = x + 15 - GameManager::GetInstance()->CameraX;
		fireRect.top = y + 100;
		fireRect.right = fireRect.left + 10;
		fireRect.bottom = fireRect.top + 10;
		break;
	case FUTON:
		fireRect.left = x + 10 - GameManager::GetInstance()->CameraX;
		fireRect.top = y;
		fireRect.right = fireRect.left + 10;
		fireRect.bottom = fireRect.top + 40;
		break;
	default:
		break;
	}
	return fireRect;
}
bool EnemyFire::OutOfScreen()
{
	if (x < 0)
		return true;
	return false;
}
void EnemyFire::Move(int _moveSpeed)
{
	x -= moveSpeed;
	x += _moveSpeed;

	/*
	switch (fireType)
	{
	case NORMAL:
		FireRect.left = x + 5;
		FireRect.top = y + 100;
		FireRect.bottom = y + 130;
		FireRect.right = x + 10;
		break;
	case SMALL:
		FireRect.left = x + 5;
		FireRect.top = y + 90;
		FireRect.bottom = y + 100;
		FireRect.right = x + 10;
		break;
	case FUTON:
		FireRect.left = x + 10;
		FireRect.top = y;
		FireRect.bottom = y + 40;
		FireRect.right = x + 15;
		break;
	default:
		break;
	}	*/
	
}