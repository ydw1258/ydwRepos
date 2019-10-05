#include<string>
#include<iostream>
#pragma warning(disable : 4996)
#define PERFECT_JUDGE 528
using namespace std;

class Vector {
public:
	int x;
	int y;
	Vector() {
		this->x = 0;
		this->y = 0;
	}
	//필요없을것 같아서 일단 뺌
	/*double vecLen() { 벡터길이
		return sqrt(x*x + y*y);
	}
	void vecNormal() { //단위벡터로 변환
		x = x / vecLen();
		y = y / vecLen();
	}*/ 
	friend Vector operator+(Vector v1, Vector v2);
	friend Vector operator-(Vector v1, Vector v2);
	friend Vector operator*(Vector v1, int d);
	friend Vector operator*(int d, Vector v1);
	//operator 연산자 오버로딩
};

//스킨 클래스는 보류
class Note {
public:
	Vector pos;
	int hp = 1;
	int track;
	int dir;
	Note(int track) {
		this->track = track;
		dir = PERFECT_JUDGE - pos.y;
		//트랙별로 pos.x를 다르게 설정함
		switch (track) //지금 작동하지 않고 있음 (상수로 대체 되어 버림)
		{
		case 1:
			pos.x = 13;
			break;
		case 2:
			pos.x = 52;
			break;
		case 3:
			pos.x = 91;
			break;
		case 4:
			pos.x = 130;
			break;
		case 5:
			pos.x = 169;
			break;
		case 6:
			pos.x = 208;
			break;
		case 7:
			pos.x = 0; //의미없지만 일단 초기화
			break;
		}
	}
	void upSpeed() {
		dir *= 2;
		pos.y = PERFECT_JUDGE - dir;
	}
	void downSpeed() {
		dir /= 2;
		pos.y = PERFECT_JUDGE - dir;
	}
};
class Effect { //노트 터질때 사용
public:
	int maxPage;
	int count = 0;
	int page = 0;
	char type;
	Vector pos;
	Effect(const int maxPage, char type) {
		this->maxPage = maxPage;
		this->type = type;
	}
};
class LEffect { //노트 터질때 사용
public:
	int maxPage;
	int count = 0;
	int page = 0;
	int track;
	Vector pos;
	LEffect(const int maxPage, int track) {
		this->maxPage = maxPage;
		this->track = track;
	}
};
class Music {
public:
	char* name;
	int level;
	int MusicNum;//음악번호
	static int MusicCount;
	Music(char* name, int level, int MusicNum) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->level = level;
		this->MusicNum = MusicNum;
		MusicCount++;
	}
};