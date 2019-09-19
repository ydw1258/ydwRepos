#include<iostream>
#include<vector>
#include<map>
#include"Student.h"

using namespace std;

int main()
{
	vector<Student*> vec;
	
	vec.push_back(new MathStudent(1, "ydw", 23, 34, 45, 56));
	vec.push_back(new MathStudent(2, "ydw2", 24, 35, 65, 336));

	vec.push_back(new HistoryStudent(3, "aa", 14, 235, 365, 336));
	vec.push_back(new HistoryStudent(4, "gga", 114, 5, 1, 6));

	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		(*it)->print();
	}
}