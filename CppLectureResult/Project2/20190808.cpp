#include<iostream>
using namespace std;

void print() { }

int main()
{
	void(*pfunc) ();

	pfunc = &print;
	(*pfunc) (); //ȣ��

	//C++ ��Ÿ��
	pfunc = print;
	pfunc();
	return 0;
}