#include<iostream>
using namespace std;

template<typename T1, typename T2>
void Print(T1 a, T2 b)
{
	cout << a << ", " << b << endl;
}

template<typename T>
void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}
int main()
{
	Print(1, 2);
	Print(1.1, 2);
	Print('a', 2);
	Print('a','b');
	Print('a', "asd");

	int a = 11;
	int b = 22;
	Swap(a, b);
	Print(a, b);

	double da = 1.1;
	double db = 2.2;
	Swap(da, db);
	Print(da, db);
}