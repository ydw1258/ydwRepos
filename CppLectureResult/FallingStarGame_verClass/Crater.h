#pragma once
class Crater
{
private:
	int x;
	int y;
	int speed;
public:
	Crater();
	~Crater();
	void move();
	void setPosition(int _x, int _y, int _speed)
	{
		x = _x;
		y = _y;
		speed = _speed;
	}
};

