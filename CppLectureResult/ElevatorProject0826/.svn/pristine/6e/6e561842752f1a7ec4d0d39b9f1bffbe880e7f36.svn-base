#include<iostream>
#include<algorithm>
using namespace std;

class Point
{
	int x;
	int y;
public:
	Point(int _x = 0, int _y = 0) 
	{
		x = _x;
		y = _y;
	}
	~Point() {};

	Point operator+(const Point& b)
	{
		Point temp(0, 0);
		temp.x = x + b.x;
		temp.y = y + b.y;
		return temp;
	}
	int GetX() const
	{
		return x;
	}
	int GetY() const
	{
		return y;
	}
	void setX(int _x)
	{
		x = _x;
	}
	void set(int _y)
	{			 
		y = _y;
	}
	void Print() const
	{
		cout << x << ", " << y << endl;
	}
	void operator++()//전위연산
	{
		x++;
		y++;
	}
	void operator++(int)//후위연산
	{
		x++;
		y++;
	}
	bool operator==(const Point& arg) const
	{
		return x == arg.x && arg.y ? true : false;
	}
	bool operator!=(const Point& arg) const
	{
		return !(*this == arg);
	}
	int operator[] (int idx) const
	{
		if (idx == 0)
			return x;
		else if (idx == 1)
			return y;
		else
			throw "No!";
	}
	operator int() const
	{
		return x;
	}
};
template<typename T>
void print(T a)
{
	cout << a << endl;
}
template<>
void print(Point a)
{
	cout << a.GetX() << " " << a.GetY() << endl;
}
class PointPtr
{
	Point *ptr;
public:
	PointPtr(Point *p) : ptr(p) {}
	~PointPtr()
	{
		delete ptr;
	}
	Point* operator->()const
	{
		return ptr;
	}
	Point& operator*() const
	{
		return *ptr;
	}
};

int main()
{
	Point pt(2, 3);
	Point *p = &pt;

	void (Point::*pf1) () const;
	pf1 = &Point::Print;
	
	//void (Point::*pf2) (int);
	//pf2 = &Point::PrintInt;

	Point pt2(2, 3);
	print(pt2);
	return 0;
}
