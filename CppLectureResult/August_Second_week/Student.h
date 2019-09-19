#pragma once
#include<iostream>
using namespace std;

class Student
{
protected : 
	int no;
	string name;
	int kor;
	int eng;
	int math;
public:
	static int staticVar;
	Student(int _no, string _name, int kor, int eng, int math);
	Student(Student& st);
	virtual void print();
	~Student();
};

class MathStudent : public Student
{
private:
	int math2;
public:
	MathStudent(int _no, string _name, int _kor, int _eng, int _math, int _math2) : Student(_no, _name, _kor, _eng, _math)
	{
		math2 = _math2;
	}
	virtual void print()
	{
		Student::print();
		cout << "수학2 : " << math2;
	}
};

class HistoryStudent : public Student
{
private : 
	int history;
public:
	HistoryStudent(int _no, string _name, int _kor, int _eng, int _math, int _history) : Student(_no, _name, _kor, _eng, _math)
	{
		history = _history;
	}
	virtual void print()
	{
		Student::print();
		cout << "역사 : " << history;
	}
};