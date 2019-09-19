#pragma once
#include<string>
#include<iostream>
using namespace std;
class Grade
{
private:
	string name;
	int kor;
	int eng;
	int math;
	in t
public:
	Grade();
	~Grade();

	void setData(string _name, int _kor, int _eng, int _math);
	void getData();
};

