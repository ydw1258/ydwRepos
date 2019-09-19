#include "Grade.h"

Grade::Grade()
{

}
void Grade::setData(string _name, int _kor, int _eng, int _math)
{
	name = _name;
	kor = _kor;
	eng = _eng;
	math = _math;
}
void Grade::getData()
{
	cout << name << " " << kor << " " << eng << " " << math << endl;
}
Grade::~Grade()
{

}