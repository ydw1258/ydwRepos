#include<iostream>
#include<vector>
#include<list>
#include<iterator>
#include<queue>
#include<string>
using namespace std;

int main()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);

	list<int> lt;
	lt.push_back(1);
	lt.push_back(2);
	lt.push_back(3);
	lt.push_back(4);

	cout << distance(v.begin(), v.end()) << endl;
	cout << distance(lt.begin(), lt.end()) << endl;
	
	string s = "abc";
	cout << s.find("1");
}