#include<iostream>
using namespace std;

void print() { }

int main()
{
	void(*pfunc) ();

	pfunc = &print;
	(*pfunc) (); //호출

	//C++ 스타일
	pfunc = print;
	pfunc();
	return 0;
}