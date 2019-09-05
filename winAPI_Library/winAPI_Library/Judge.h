#pragma once
#include<Windows.h>

class Judge
{
public:
	Judge();
	~Judge();
	bool RectCollisionCheck(RECT rc, RECT rc2);
};

