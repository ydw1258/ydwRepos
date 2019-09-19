#include<iostream>
#include<string>
using namespace std;

template<typename RetType, typename ArgType>
class Functor
{
public:
	RetType operator() (ArgType data)
	{
		cout << data << endl;
		return RetType();
	}
};
template<typename T1, typename T2>
struct Pair
{
	T1 first;
	T2 second;
	Pair(const T1& ft, const T2& sd) : first(ft), second(sd) {}
};

int main()
{
	Functor<void, int> functor1;
	functor1(10);
	Functor<bool, string> functor2;
	functor2("Hello");

	Pair<int, int> p1(10, 20);
	cout << p1.first << ',' << p1.second << endl;
	Pair<int, string> p2(10, "stst");
	cout << p2.first << ',' << p2.second << endl;

	return 0;
}