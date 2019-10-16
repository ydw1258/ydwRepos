#pragma once
#include "Vector2D.h"

template<typename T>
class Transform
{
private:
	Vector2D<T> pos;
public:
	//void Rotation();


	Transform();
	~Transform();
};

