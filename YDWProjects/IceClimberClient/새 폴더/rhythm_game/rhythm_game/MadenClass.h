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
	//�ʿ������ ���Ƽ� �ϴ� ��
	/*double vecLen() { ���ͱ���
		return sqrt(x*x + y*y);
	}
	void vecNormal() { //�������ͷ� ��ȯ
		x = x / vecLen();
		y = y / vecLen();
	}*/ 
	friend Vector operator+(Vector v1, Vector v2);
	friend Vector operator-(Vector v1, Vector v2);
	friend Vector operator*(Vector v1, int d);
	friend Vector operator*(int d, Vector v1);
	//operator ������ �����ε�
};

//��Ų Ŭ������ ����
class Note {
public:
	Vector pos;
	int hp = 1;
	int track;
	int dir;
	Note(int track) {
		this->track = track;
		dir = PERFECT_JUDGE - pos.y;
		//Ʈ������ pos.x�� �ٸ��� ������
		switch (track) //���� �۵����� �ʰ� ���� (����� ��ü �Ǿ� ����)
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
			pos.x = 0; //�ǹ̾����� �ϴ� �ʱ�ȭ
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
class Effect { //��Ʈ ������ ���
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
class LEffect { //��Ʈ ������ ���
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
	int MusicNum;//���ǹ�ȣ
	static int MusicCount;
	Music(char* name, int level, int MusicNum) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->level = level;
		this->MusicNum = MusicNum;
		MusicCount++;
	}
};