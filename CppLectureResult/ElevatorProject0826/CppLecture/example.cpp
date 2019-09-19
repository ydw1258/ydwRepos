#include<iostream>
using namespace std;
#include<algorithm>

class Functor1
{
public:
	void operator() (int n)
	{
		cout << n << " ";
	}
};
class Functor2
{
public:
	void operator() (int n)
	{
		cout << n * n << " ";
	}
};

class Functor3
{
public:
	void operator() (int n)
	{
		cout << "Á¤¼ö : " << n << endl;
	}
};

int main()
{
	int arr[5] = { 10, 20, 30, 40, 50 };

	for_each(arr, arr + 5, Functor1());
	cout << endl;
	for_each(arr, arr + 5, Functor2());
	cout << endl;
	for_each(arr, arr + 5, Functor3());
	cout << endl;
}