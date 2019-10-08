#pragma once
#include "Player.h"
#include "SpriteRenderer.h"

class InGameManager
{
public:
	Player player;
	SpriteRenderer BackgroundSprite;
	
	int offsetY;

	void Init(HDC hdc);
	void Draw();
	void DrawBackground();
	void DrawPlayer();
	InGameManager();
	~InGameManager();
};

