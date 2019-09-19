#include "Student.h"

int Student::staticVar = 10;

Student::Student(int _no, string _name, int _kor, int _eng, int _math)
{
	no = _no;
	name.reserve(256);
	name = _name;

	kor = _kor;
	eng = _eng;
	math = _math;
}
Student::Student(Student& st)
{
	no = st.no;
	name.reserve(256);
	name = st.name;
}
Student::~Student()
{

}

void Student::print()
{
	cout << no << endl;
	cout << name.c_str() << endl;
	cout <<"국어 : " << kor << endl;
	cout << "영어 : " << eng << endl;
	cout << "수학 : " << math << endl;
}
