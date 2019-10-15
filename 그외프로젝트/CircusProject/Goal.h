#pragma once
#include"ResourceManager.h"
#include "SpriteRenderer.h"

class Goal
{
	int x;
	int y;
	SpriteRenderer sprite;
public:
	void Init(int _x, int _y);
	void Draw(HDC hdc);
	Goal();
	~Goal();
};

