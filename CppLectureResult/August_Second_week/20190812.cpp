#include<iostream>
#include"Student.h"
using namespace std;

class Point
{
public :
	int x;
	int y;

	void AddPoint(Point other)
	{
		x += other.x;
		y += other.y;
	}
	//static���� ¥����
	static void AddPoint(Point* ptSour, Point ptOther)
	{
		ptSour->x += ptOther.x;
		ptSour->y += ptOther.y;
	}
};
int main()
{
	char ch[5] = "name";
	
}