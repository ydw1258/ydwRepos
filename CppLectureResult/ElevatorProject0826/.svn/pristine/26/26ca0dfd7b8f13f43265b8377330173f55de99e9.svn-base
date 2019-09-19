#include<iostream>
#include<string>
using namespace std;

template<typename T>
class Array
{
	T * buf;
	int size;
	int capacity;
public:
	explicit Array(int cap = 100) : buf(0), size(0), capacity(cap)
	{
		buf = new T[capacity];
	}
	~Array() { delete[] buf; }
	void Add(T data)
	{
		buf[size++] = data;
	}
	T operator[] (int idx) const
	{
		return buf[idx];
	}
	int GetSize() const
	{
		return size;
	}
};
template<typename T>
class ObjectInfo
{
	T data;
public:
	ObjectInfo(const T& d) : data(d) {}

	void Print()
	{
		cout << "타입 : " << typeid (data).name() << endl;
		cout << "크기 : " << sizeof(data) << endl;
		cout << "값 : " << data << endl;
		cout << endl;
	}
};

template<typename IterT, typename Func>
void For_each(IterT begin, IterT end, Func pf)
{
	while (begin != end)
	{
		pf(*begin++);
	}
}

void PrintInt(int data)
{
	cout << data << " ";
}
void PrintString(string data)
{
	cout << data << " ";
}

int main()
{
	/*Array<int> iarr;
	iarr.Add(10);
	iarr.Add(20);
	iarr.Add(30);

	for (int i = 0; i < iarr.GetSize(); i++)
		cout << iarr[i] << " ";
	cout << endl;

	Array<double> darr;
	darr.Add(10.12);
	darr.Add(20.12);
	darr.Add(30.12);
	

	for (int i = 0; i < darr.GetSize(); i++)
		cout << darr[i] << " ";
	cout << endl;
	

	Array<string> sarr;
	sarr.Add("asd");
	sarr.Add("strstst");
	sarr.Add("abc");

	for (int i = 0; i < sarr.GetSize(); i++)
		cout << sarr[i] << " ";
	cout << endl;
	*/
	/*
	ObjectInfo<int> d1(10);
	d1.Print();
	ObjectInfo<double> d2(5.5);
	d2.Print();
	ObjectInfo<string> d3("Hello1");
	d3.Print();
	*/

	int arr[5] = { 10, 20, 30, 40, 50 };
	For_each(arr, arr + 5, PrintInt);
	cout << endl;

	string sarr[3] = { "abc", "abcde", "hello" };
	For_each(sarr, sarr + 3, PrintString);
	cout << endl;
}