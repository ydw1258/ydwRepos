#include"MadenClass.h"

Vector operator+(Vector v1, Vector v2) {
	Vector temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	return temp;
}
Vector operator-(Vector v1, Vector v2) {
	Vector temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	return temp;
}
Vector operator*(Vector v1, int d) {
	Vector temp;
	temp.x = d * v1.x;
	temp.y = d * v1.y;
	return temp;
}
Vector operator*(int d, Vector v1) {
	Vector temp;
	temp.x = d * v1.x;
	temp.y = d * v1.y;
	return temp;
}
//Note
//Effect