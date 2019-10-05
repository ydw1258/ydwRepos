#include <WinSock2.h>
#include "setkeyfps.h"
//#pragma warning (disable : 4996)

#include<list>
#include "MadenClass.h"
#include"fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")
using namespace FMOD;

#include<iostream>
#include <WinSock2.h>
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")
//#pragma warning(disable : 4996)

#include <my_global.h>
#include <mysql.h>
using namespace std;

#define HOST	"localhost"
#define ID		"root"
#define PASSWD	"apmsetup"
#define DBNAME	"rhythm"
#define CHOP(x) x[strlen(x) - 1] = ' '

enum SOUNDKIND {
	SD_A,
	SD_B,
	SD_C,
	SD_D,
	SD_E,
	SD_F,
	SD_G,
	SD_H,
	SD_END
};
System*   g_pSystem;
Sound*    g_pSound[SD_END];//���尹����ŭ �ʿ�
Channel* g_pChannel[2];//���ÿ� ��µ� ���� ��ŭ �ʿ�

//��Ʈ��� ����Ʈ
list<Note> nList;
list<Note>::iterator nIt;
//���Ǽ� ��� ����Ʈ
list<Music*> mList;
list<Music*>::iterator mIt;
//��������Ʈ
list<Effect*> effList;
list<Effect*>::iterator effIt;
//����Ʈ ����Ʈ(Good�̻� ������ ����)
list<LEffect*> leffList;
list<LEffect*>::iterator leffIt;

int Music::MusicCount = 0;
void Notepattern(int pattern, int MusicNum);
#define PERFECT_JUDGE 528
//���� ���̵�
#define KOOL_JUDGE 3
#define COOL_JUDGE 8
#define GOOD_JUDGE 20
#define MISS_JUDGE 30
//������ ���ھ�
#define KOOLSCORE 500
#define COOLSCORE 300
#define GOODSCORE 50
#define MAX_LIFE 300

int printNumber(int comboCount) { //�ڸ��� ��ȯ�Լ�
	if (comboCount / 10 < 1) {
		return 1;
	}
	else if (comboCount / 100 < 1) {
		return 2;
	}
	else if (comboCount / 1000 < 1) {
		return 3;
	}
	else if (comboCount / 10000 < 1) {
		return 4;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC	hdc;
	//HDC �׸������ ��� ��Ҹ� ������ �ִ� ����ü, HDC��ü�� SelectObject�� �̿��Ͽ� HBITMAP����ü�� ��ü�� �����ϰ� �ٷ� �� �ִ�.
	//�̹��� �׸��� ���� (���߿� ������ ���������� ������)
	int x;
	int y; //���콺 x, y��ǥ ���� ���� ����
	static int bgstate = 0; //0�϶� ��� 1�϶� �÷���
	static int selCur = 0; //�� ����â Ŀ���̹��� �ѱ�� ����
	static int pattern = 0; //0�븻 1�̷� 2����
	static int noteSpeed = 1;
	static int comboCount = 0;
	static int maxCombo = 0;
	static int totalNote = -1; //�ѳ�Ʈ ����(�� Ŭ����� ������ ���� ��Ʈ �Ѱ� ������ -1�� �ʱ�ȭ)
	static int judgeCount = 0;
	static int score = 0;
	static int lifeGauge = MAX_LIFE;
	//����Ʈ â�� ���ư��� ���� ��ǥ�� �ٷ�� ����
	static int RightslideBarX = 1000;
	static int LeftslideBarX = -500;
	static int MusicNum = 0; //���â���� ����Ʈ Ŀ�� ��ġ�� ����Ʈ â�� ���(S, A+��) �ߴ� ȿ�� �ٷ�� ����
	static int koolCount = 0;
	static int coolCount = 0;
	static int goodCount = 0;
	static int missCount = 0;
	static int failCount = 0;
	static int musicSeq = 0; //���� Ŀ��

	Music *music = new Music("Default", 0, 0); //�ʱ�ȭ ���� ������ �׳� ������
	
	MYSQL       *connection = NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_state;
	
	switch (message) {
	case WM_CREATE:
		hdc = GetDC(hwnd);

		music = new Music("Diavolo", 12, 1);
		mList.push_back(music);
		music = new Music("FreedomDive", 14, 2);
		mList.push_back(music);

		//Ÿ�̸Ӽ���
		SetTimer(hwnd, 1, 3, NULL);//���, ������ ����Ʈ ��� Ÿ�̸�
		SetTimer(hwnd, 2, 1, NULL); //���� ����Ʈ 
		SetTimer(hwnd, 3, 30, NULL); //����Ʈ Ŀ��
		//��Ʈ�� �ε�
		for (i = 0; i < 50; i++) {
			hMemdc[i] = CreateCompatibleDC(hdc); //������ DC(device context)�� hMemdc�� ����
			hBitmap[i] = LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1 + i));
			//hdc��ü�� hBitmap�� ���� ��Ʈ���� ������ ������
			SelectObject(hMemdc[i], hBitmap[i]); //hMemdc[i]�� hBi
												 //tmap[i]�� ��Ʈ�� ������ �ְԵǸ鼭 hMemdc�� ũ�Ⱑ ��Ʈ���� ũ�⿡ ��������.
		}

		hOffdc = CreateCompatibleDC(hdc); //hdc�� ������ �̸� hOffdc�� �Ǿ ���߿� ���(�ȱ׷��� ������)
		hOffBmp = CreateCompatibleBitmap(hdc, 1000, 688); //��ü��ȭ������, WinMain �ҽ����Ͽ��� Width,Height���� ����� ��
		SelectObject(hOffdc, hOffBmp);
		
		System_Create(&g_pSystem); //���� �ý��� ����
		g_pSystem->init(2, FMOD_INIT_NORMAL, NULL); //���� �ý��� �⺻����
		g_pChannel[0]->setVolume(1);
		g_pChannel[1]->setVolume(100);
		enum { BGM, DECIDE, OPTION, DIAVOLO, rbyFAILED, rbyCLEAR, Freedom_Dive, rbyMOVE }; 
		//FMOD�� ���� �߰��� ���ǵ��� �ε��� ������ enum���� ���ȭ �Ͽ� ����

		g_pSystem->createSound("resource\\SELECT_BGM.OGG", FMOD_LOOP_NORMAL, NULL, &g_pSound[0]); //||FMOD_HARDWARE���� ������ �ȵ�
		g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[BGM], false, &g_pChannel[0]); //0ä�ο�, 0���� ���(BGM)
		g_pSystem->createSound("resource\\DECIDE.OGG", FMOD_LOOP_OFF, NULL, &g_pSound[DECIDE]);
		g_pSystem->createSound("resource\\OPTION.OGG", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[OPTION]);
		g_pSystem->createSound("resource\\DIAVOLO.MP3", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[DIAVOLO]);
		g_pSystem->createSound("resource\\rbyFAILED.OGG", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[rbyFAILED]);
		g_pSystem->createSound("resource\\rbyCLEAR.OGG", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[rbyCLEAR]);
		g_pSystem->createSound("resource\\Freedom Dive.MP3", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[Freedom_Dive]);
		g_pSystem->createSound("resource\\rbyMOVE.OGG", FMOD_LOOP_OFF || FMOD_HARDWARE, NULL, &g_pSound[rbyMOVE]);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		Key_Input(VK_UP, &UP_KEY);
		Key_Input(VK_LEFT, &LEFT_KEY);
		Key_Input(VK_DOWN, &DOWN_KEY);
		Key_Input(VK_RIGHT, &RIGHT_KEY);
		Key_Input(VK_ESCAPE, &ESC);
		Key_Input(VK_RETURN, &ENTER);
		Key_Input('S', &S_KEY);
		Key_Input('D', &D_KEY);
		Key_Input('F', &F_KEY);
		Key_Input('J', &J_KEY);
		Key_Input('K', &K_KEY);
		Key_Input('L', &L_KEY);

		if (bgstate == 0) {
			//����Ű�� ������ Ŀ�� y�ຯ���� �̵���Ű�� ȿ������ ���
			if (UP_KEY == PUSH_KEY || LEFT_KEY == PUSH_KEY) {
				musicSeq -= 1;
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[rbyMOVE], false, &g_pChannel[1]);
			}
			if (DOWN_KEY == PUSH_KEY || RIGHT_KEY == PUSH_KEY) {
				musicSeq += 1;
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[rbyMOVE], false, &g_pChannel[1]);
			}
			if (musicSeq > 1)  //�����ǰ��� - 1��(������)�� �Ѿ�� ó������
				musicSeq = 0;
			if (musicSeq < 0)
				musicSeq = 1; //ó������ ������ �ö󰡸� ����������
			if (wParam == 3) {
				selCur++;
				if (selCur > 5)//����Ʈ �̹��� ��� 0 ~ 5(6��¥��)
					selCur = 1;
			}
			//�׸�
			BitBlt(hdc, 0, 0, 1024, 700, hOffdc, 0, 0, SRCCOPY);
			BitBlt(hOffdc, 0, 0, 1024, 700, hMemdc[2], 0, 0, SRCCOPY); //����ȭ�� blackboard
			BitBlt(hOffdc, 0, 0, 800, 600, hMemdc[0], 0, 0, SRCCOPY); //���ȭ��
			BitBlt(hOffdc, 530, 238, 28, 28, hMemdc[0], 810, 0, SRCCOPY); //1p �׳� �̹���
			BitBlt(hOffdc, 563, 238, 28, 28, hMemdc[0], 870, pattern * 28, SRCCOPY); //Ű ���� ���� �̹��� ����
			BitBlt(hOffdc, 10, 70 + 40 * musicSeq, 99, 99, hMemdc[6], 9 + 101 * selCur, 3, SRCPAINT); //����Ʈ Ŀ�� ����Ʈ

			for (mIt = mList.begin(); mIt != mList.end(); mIt++) {
				TextOut(hOffdc, 100, 70 + 40 * (*mIt)->MusicNum, (*mIt)->name, strlen((*mIt)->name)); //���ڿ��� ���.
				sprintf(buffer, "%d", (*mIt)->level); //563 238
				TextOut(hOffdc, 200, 70 + 40 * (*mIt)->MusicNum, buffer, 2);
			}
			if (ENTER == PUSH_KEY) //���� ������ ���� ����
			{
				bgstate = 1;
				//Ŀ�� ��ġ�� ���� �ٸ��� ���
				if (musicSeq == 0) //0 ���� ����
				{ 
					Notepattern(pattern, musicSeq); //���õ� �� ȣ�� (��Ʈ����)
					g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[DIAVOLO], false, &g_pChannel[1]);
					g_pChannel[1]->setVolume(4);
				}
				else if (musicSeq == 1) //1 ���� ����
				{
					Notepattern(pattern, musicSeq);
					g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[Freedom_Dive], false, &g_pChannel[1]);
					g_pChannel[1]->setVolume(4);
				}
				for (nIt = nList.begin(); nIt != nList.end(); nIt++) { //��ü ��Ʈ�� ���
					totalNote++;
				}
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[DECIDE], false, &g_pChannel[0]); //��� ȿ����
				g_pChannel[0]->setVolume(1);
				//������ �ʱ�ȭ
				comboCount = 0;
				maxCombo = 0;
				score = 0;
				lifeGauge = MAX_LIFE;
			}
		}
		//�������� ���� ó�� (���������� ������ ������ ������ FAILó��)

		else if (bgstate == 1) { // ��� ����
			
			if (lifeGauge <= 0) { //����� ó��
				BitBlt(hOffdc, 0, 0, 800, 600, hMemdc[4], 0, 0, SRCCOPY);
				g_pChannel[1]->stop();
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[4], false, &g_pChannel[0]); //RETIRE ȿ����
				RightslideBarX = 1000;
				LeftslideBarX = -500;
				bgstate = 2;
				break;
			}
			//��Ų �̹��� ��ü ���

			//��� ����
			if (UP_KEY == PUSH_KEY) {
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[OPTION], false, &g_pChannel[0]);
				if (noteSpeed == 32)
					break;
				for (nIt = nList.begin(); nIt != nList.end(); nIt++) {
					(nIt)->dir = PERFECT_JUDGE - (nIt)->pos.y; //dir (�������� ��Ʈ���� �Ÿ��� ����)
					(*nIt).upSpeed(); //�Ÿ�����
				}
				noteSpeed *= 2; //�Ÿ������Ѹ�ŭ �ӵ��� ���� (���� ������)
			}
			if (DOWN_KEY == PUSH_KEY) { // �ӵ����� �ݴ�
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[OPTION], false, &g_pChannel[0]);
				if (noteSpeed == 1)
					break;
				for (nIt = nList.begin(); nIt != nList.end(); nIt++) {
					(nIt)->dir = PERFECT_JUDGE - (nIt)->pos.y;
					(*nIt).downSpeed();
				}
				noteSpeed /= 2;
			}
			// �������� ������ ��ȭ�� 
			// �������� +�Ǵ� ����
			int KoolLife = 2;
			int coolLife = 1;
			// �������� -�Ǵ� ����
			int failLife = 8;
			int missLife = 7;

			//Ű�� ���� ó��
			if (S_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 13, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 1)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 1);
							leffList.push_back(l);
							score += KOOLSCORE;
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 1);
							leffList.push_back(l);
							score += COOLSCORE;
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 1);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= 7;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}
			if (D_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 52, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 2)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 2);
							leffList.push_back(l);
							score += KOOLSCORE;
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = 300;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 2);
							leffList.push_back(l);
							score += COOLSCORE;
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 2);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= missLife;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}
			if (F_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 91, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 3)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 3);
							leffList.push_back(l);
							score += KOOLSCORE;
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 3);
							leffList.push_back(l);
							score += COOLSCORE;
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 3);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= missLife;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}
			if (J_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 130, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 4)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 4);
							leffList.push_back(l);
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += KOOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 4);
							leffList.push_back(l);
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += COOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 4);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= missLife;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}
			if (K_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 169, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 5)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 5);
							leffList.push_back(l);
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += KOOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 5);
							leffList.push_back(l);
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += COOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 5);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= missLife;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}
			if (L_KEY == PUSH_KEY) {
				BitBlt(hOffdc, 208, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt->pos.y >= PERFECT_JUDGE - MISS_JUDGE * noteSpeed && (nIt != nList.end() && nIt->track == 6)) {
						if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * KOOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * KOOL_JUDGE) {//Kool
							comboCount++;
							koolCount++;
							Effect *k = new Effect(6, 'k');
							effList.clear();
							effList.push_back(k);
							LEffect *l = new LEffect(10, 6);
							leffList.push_back(l);
							lifeGauge += KoolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += KOOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * COOL_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * COOL_JUDGE) {//Cool
							comboCount++;
							coolCount++;
							Effect *c = new Effect(6, 'c');
							effList.clear();
							effList.push_back(c);
							LEffect *l = new LEffect(10, 6);
							leffList.push_back(l);
							lifeGauge += coolLife;
							if (lifeGauge > MAX_LIFE)
								lifeGauge = MAX_LIFE;
							score += COOLSCORE;
						}
						else if (nIt->pos.y < PERFECT_JUDGE + noteSpeed * GOOD_JUDGE && nIt->pos.y > PERFECT_JUDGE - noteSpeed * GOOD_JUDGE) { //Good
							comboCount++;
							goodCount++;
							Effect *g = new Effect(6, 'g');
							effList.clear();
							effList.push_back(g);
							LEffect *l = new LEffect(10, 6);
							leffList.push_back(l);
							score += GOODSCORE;
						}
						else { //miss
							comboCount = 0;
							missCount++;
							Effect *m = new Effect(6, 'm');
							effList.clear();
							effList.push_back(m);
							lifeGauge -= missLife;
						}//500 528 �������� 600
						nIt = nList.erase(nIt);
						break;
					}
					else
						nIt++;
				}
			}

			for (nIt = nList.begin(); nIt != nList.end();) { //fail����
					if (nIt->pos.y >= PERFECT_JUDGE + 10 * noteSpeed) {
						//Fail����
						failCount++;
						Effect *f = new Effect(6, 'f');
						effList.clear();
						effList.push_back(f);
						nIt = nList.erase(nIt);
						comboCount = 0;
						lifeGauge -= failLife;
					}
					else
						nIt++;
			}
			if (wParam == 2) { //��Ʈ �������� �ð� ���� Ÿ�̸�
							   //��Ʈ ����Ʈ���� ���
				for (nIt = nList.begin(); nIt != nList.end(); nIt++) {
					(nIt)->pos.y += noteSpeed; //�ӵ� bgm
				}
			}
			//���ھ� ��� (10�� �ڸ�)
			BitBlt(hOffdc, 42, 635, 12, 15, hMemdc[1], 14 + (score / 100000) * 13, 656, SRCCOPY);
			BitBlt(hOffdc, 55, 635, 12, 15, hMemdc[1], 14 + ((score % 100000) / 10000) * 13, 656, SRCCOPY);
			BitBlt(hOffdc, 68, 635, 12, 15, hMemdc[1], 14 + ((score % 10000) / 1000) * 13, 656, SRCCOPY);
			BitBlt(hOffdc, 81, 635, 12, 15, hMemdc[1], 14 + (score % 1000 / 100) * 13, 656, SRCCOPY);
			//1���ڸ��� 10�� �ڸ��� ������ �ʾƼ� 0���� ó��
			BitBlt(hOffdc, 94, 635, 12, 15, hMemdc[1], 14, 656, SRCCOPY);
			BitBlt(hOffdc, 107, 635, 12, 15, hMemdc[1], 14, 656, SRCCOPY);

			//maxCombo���
			BitBlt(hOffdc, 166, 610, 12, 15, hMemdc[1], 14 + (maxCombo / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 178, 610, 12, 15, hMemdc[1], 14 + ((maxCombo % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 190, 610, 12, 15, hMemdc[1], 14 + ((maxCombo % 100) / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 202, 610, 12, 15, hMemdc[1], 14 + (maxCombo % 10) * 13, 640, SRCCOPY);
			
			BitBlt(hOffdc, 260, 560, 542, 87, hMemdc[1], 1, 688, SRCCOPY); //�÷����� ������
			//�÷����� ���� ���� ���
			BitBlt(hOffdc, 320, 580, 12, 15, hMemdc[1], 14 + (koolCount / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 333, 580, 12, 15, hMemdc[1], 14 + ((koolCount % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 346, 580, 12, 15, hMemdc[1], 14 + ((koolCount % 100) / 10)* 13, 640, SRCCOPY);
			BitBlt(hOffdc, 359, 580, 12, 15, hMemdc[1], 14 + (koolCount % 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 410, 580, 12, 15, hMemdc[1], 14 + (coolCount / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 423, 580, 12, 15, hMemdc[1], 14 + ((coolCount % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 436, 580, 12, 15, hMemdc[1], 14 + ((coolCount % 100) / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 449, 580, 12, 15, hMemdc[1], 14 + (coolCount % 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 500, 580, 12, 15, hMemdc[1], 14 + (goodCount / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 513, 580, 12, 15, hMemdc[1], 14 + ((goodCount % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 526, 580, 12, 15, hMemdc[1], 14 + ((goodCount % 100) / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 539, 580, 12, 15, hMemdc[1], 14 + (goodCount % 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 320, 620, 12, 15, hMemdc[1], 14 + (missCount / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 333, 620, 12, 15, hMemdc[1], 14 + ((missCount % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 346, 620, 12, 15, hMemdc[1], 14 + ((missCount % 100) / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 359, 620, 12, 15, hMemdc[1], 14 + (missCount % 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 500, 620, 12, 15, hMemdc[1], 14 + (failCount / 1000) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 513, 620, 12, 15, hMemdc[1], 14 + ((failCount % 1000) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 526, 620, 12, 15, hMemdc[1], 14 + ((failCount % 100) / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 539, 620, 12, 15, hMemdc[1], 14 + (failCount % 10) * 13, 640, SRCCOPY);
			//HP
			BitBlt(hOffdc, 665, 580, 12, 15, hMemdc[1], 14 + ((lifeGauge / MAX_LIFE * 100) / 100) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 678, 580, 12, 15, hMemdc[1], 14 + ( (int)((double)lifeGauge / MAX_LIFE * 100 ) % 100) / 10 * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 691, 580, 12, 15, hMemdc[1], 14 + ((int)((double)lifeGauge / MAX_LIFE * 100) % 10) * 13, 640, SRCCOPY);
			//��� ���
			BitBlt(hOffdc, 707, 620, 12, 15, hMemdc[1], 14 + (noteSpeed / 10) * 13, 640, SRCCOPY);
			BitBlt(hOffdc, 720, 620, 12, 15, hMemdc[1], 14 + (noteSpeed % 10) * 13, 640, SRCCOPY);
			if (ESC == PUSH_KEY) //�����߿� esc�� ������ �ߵ� ����
			{
				//�뷡�� ������Ű�� �÷��̿� �ʿ��� ������ �ʱ�ȭ
				g_pChannel[1]->stop();
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[rbyFAILED], false, &g_pChannel[0]); //RETIRE ȿ����
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt != nList.end())
						nIt = nList.erase(nIt);
					else
						nIt++;
				}
				RightslideBarX = 1000;
				LeftslideBarX = -500;
				bgstate = 2; //���ȭ������
				break;
			}
			if (wParam == 1) {
				if (comboCount != 0) { //�޺��� 0�� �ƴϸ� �޺����
					switch (printNumber(comboCount)) {
					case 1:
						BitBlt(hOffdc, 100, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount), 557, SRCPAINT);
						break;
					case 2:
						BitBlt(hOffdc, 84, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount / 10), 557, SRCPAINT);
						BitBlt(hOffdc, 116, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount % 10), 557, SRCPAINT);
						break;
					case 3:
						BitBlt(hOffdc, 68, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount / 100), 557, SRCPAINT);
						BitBlt(hOffdc, 100, 150, 32, 32, hMemdc[1], 34 + 33 * ((comboCount % 100) / 10), 557, SRCPAINT);
						BitBlt(hOffdc, 132, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount % 10), 557, SRCPAINT);
						break;
					case 4:
						BitBlt(hOffdc, 52, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount / 1000), 557, SRCPAINT);
						BitBlt(hOffdc, 84, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount % 1000 / 100), 557, SRCPAINT);
						BitBlt(hOffdc, 116, 150, 32, 32, hMemdc[1], 34 + 33 * ((comboCount % 100) / 10), 557, SRCPAINT);
						BitBlt(hOffdc, 148, 150, 32, 32, hMemdc[1], 34 + 33 * (comboCount % 10), 557, SRCPAINT);
						break;
					}
				}
				BitBlt(hdc, 0, 0, 1024, 700, hOffdc, 0, 0, SRCCOPY);
				BitBlt(hOffdc, 0, 0, 1024, 700, hMemdc[2], 0, 0, SRCCOPY);

				//������ ������ Ű���� �ҿö�� �ִ� ����Ʈ ó��
				if (S_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 13, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				if (D_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 52, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				if (F_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 91, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				if (J_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 130, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				if (K_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 169, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				if (L_KEY == HOLD_KEY) {
					BitBlt(hOffdc, 208, 120, 38, 400, hMemdc[1], 725, 1, SRCCOPY);
				}
				//�߱� ����Ʈ ����Ʈ ó��
				for (leffIt = leffList.begin(); leffIt != leffList.end();) {
					if ((*leffIt)->track == 1) {
						BitBlt(hOffdc, -18, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //1�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else if ((*leffIt)->track == 2) {
						BitBlt(hOffdc, 20, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //2�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else if ((*leffIt)->track == 3) {
						BitBlt(hOffdc, 58, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //3�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else if ((*leffIt)->track == 4) {
						BitBlt(hOffdc, 96, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //4�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else if ((*leffIt)->track == 5) {
						BitBlt(hOffdc, 134, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //5�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else if ((*leffIt)->track == 6) {
						BitBlt(hOffdc, 172, 460, 96, 96, hMemdc[1], 1 + 97 * ((*leffIt)->count), 777, SRCPAINT); //6�� �߱�
						(*leffIt)->count++;
						if ((*leffIt)->maxPage - 1 == (*leffIt)->count) { //������ �������̸� ����
							leffIt = leffList.erase(leffIt);
						}
						else
							leffIt++;
					}
					else
						nIt++;
				}
				//���� ����Ʈ ����Ʈ ó��
				for (effIt = effList.begin(); effIt != effList.end();) {
					if ((*effIt)->type == 'k') {
						judgeCount++;
						BitBlt(hOffdc, 70, 460, 128, 32, hMemdc[1], 1, 347 + (33)*(*effIt)->count, SRCPAINT);
						if (judgeCount > 3)//��������Ʈ�� �ʹ� ���� ������� ���� ����
						{
							judgeCount = 0;
							(*effIt)->count++;
						}
						if ((*effIt)->maxPage - 1 == (*effIt)->count) { //������ �������̸� ����
							effIt = effList.erase(effIt);
						}
						else
							effIt++;
					}
					else if ((*effIt)->type == 'c') {
						judgeCount++;
						BitBlt(hOffdc, 70, 460, 128, 32, hMemdc[1], 130, 347 + (33)*(*effIt)->count, SRCPAINT);
						if (judgeCount > 3)//��������Ʈ�� �ʹ� ���� ������� ���� ����
						{
							judgeCount = 0;
							(*effIt)->count++;
						}
						if ((*effIt)->maxPage - 1 == (*effIt)->count) { //������ �������̸� ����
							effIt = effList.erase(effIt);
						}
						else
							effIt++;
					}
					else if ((*effIt)->type == 'g') {
						judgeCount++;
						BitBlt(hOffdc, 70, 460, 128, 32, hMemdc[1], 259, 347 + (33)*(*effIt)->count, SRCPAINT);
						if (judgeCount > 3)//��������Ʈ�� �ʹ� ���� ������� ���� ����
						{
							judgeCount = 0;
							(*effIt)->count++;
						}
						if ((*effIt)->maxPage - 1 == (*effIt)->count) { //������ �������̸� ����
							effIt = effList.erase(effIt);
						}
						else
							effIt++;
					}
					else if ((*effIt)->type == 'm') {
						judgeCount++;
						BitBlt(hOffdc, 70, 460, 128, 32, hMemdc[1], 388, 347 + (33)*(*effIt)->count, SRCPAINT);
						if (judgeCount > 3)//��������Ʈ�� �ʹ� ���� ������� ���� ����
						{
							judgeCount = 0;
							(*effIt)->count++;
						}
						if ((*effIt)->maxPage - 1 == (*effIt)->count) { //������ �������̸� ����
							effIt = effList.erase(effIt);
						}
						else
							effIt++;
					}
					else if ((*effIt)->type == 'f')
					{
						judgeCount++;
						BitBlt(hOffdc, 70, 460, 128, 32, hMemdc[1], 517, 347 + (33)*(*effIt)->count, SRCPAINT);
						if (judgeCount > 3)//��������Ʈ�� �ʹ� ���� ������� ���� ����
						{
							judgeCount = 0;
							(*effIt)->count++;
						}
						if ((*effIt)->maxPage - 1 == (*effIt)->count) { //������ �������̸� ����
							effIt = effList.erase(effIt);
						}
						else
							effIt++;
					}
					else
						nIt++;
				}

				//��Ʈ �̹���
				for (nIt = nList.begin(); nIt != nList.end(); nIt++) {
					switch ((nIt)->track) {
					case 1:
						BitBlt(hOffdc, 13, (nIt)->pos.y, 36, 8, hMemdc[1], 259, 1, SRCCOPY); //1��Ʈ�� 
						break;
					case 2:
						BitBlt(hOffdc, 52, (nIt)->pos.y, 36, 8, hMemdc[1], 296, 1, SRCCOPY); //2�� Ʈ��
						break;
					case 3:
						BitBlt(hOffdc, 92, (nIt)->pos.y, 36, 8, hMemdc[1], 259, 1, SRCCOPY); //3��Ʈ�� 
						break;
					case 4:
						BitBlt(hOffdc, 130, (nIt)->pos.y, 36, 8, hMemdc[1], 259, 1, SRCCOPY); //4��Ʈ�� 
						break;
					case 5:
						BitBlt(hOffdc, 169, (nIt)->pos.y, 36, 8, hMemdc[1], 296, 1, SRCCOPY); //5�� Ʈ��
						break;
					case 6:
						BitBlt(hOffdc, 208, (nIt)->pos.y, 36, 8, hMemdc[1], 259, 1, SRCCOPY); //6��Ʈ�� 
						break;
					case 7: //���� ������ ����
						if (nIt->pos.y > PERFECT_JUDGE)
						{
							g_pChannel[1]->stop();
							g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[rbyCLEAR], false, &g_pChannel[0]); //CLEAR ȿ����
							RightslideBarX = 1000;
							LeftslideBarX = -500;
							bgstate = 3;
						}
						break;
					}
				}
			}
			
			if (maxCombo < comboCount)
				maxCombo = comboCount;

			//�����ǥ, ũ��, ������ġ
			BitBlt(hOffdc, 0, 62, 11, 529, hMemdc[1], 888, 1, SRCCOPY); //������ ������
			BitBlt(hOffdc, 3, 83, 5, 450 - lifeGauge*1.5, hMemdc[2], 888, 1, SRCCOPY);
			//������ ���̿� blackboard�� �Ϻ�, lifeGauge�� ���������� �������� ǥ���Ͽ� ������ �������� ����
			BitBlt(hOffdc, 0, 522, 258, 140, hMemdc[1], 0, 0, SRCCOPY); //��Ų ��ư��
			BitBlt(hOffdc, 13, 520, 232, 10, hMemdc[1], 417, 1, SRCCOPY); //������
		}
		else if (bgstate == 2) { //����� 
			RightslideBarX -= 10;
			LeftslideBarX += 10;
			if (RightslideBarX < 340)
				RightslideBarX = 340;
			if (LeftslideBarX > 340)
				LeftslideBarX = 340;
			BitBlt(hdc, 0, 0, 1024, 700, hOffdc, 0, 0, SRCCOPY);
			BitBlt(hOffdc, 0, 0, 1024, 700, hMemdc[2], 0, 0, SRCCOPY);
			BitBlt(hOffdc, 100, 20, 800, 600, hMemdc[4], 0, 0, SRCCOPY);

			//resultâ�� ���ƿ��� ���� ��
			BitBlt(hOffdc, RightslideBarX, 40, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 90, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, RightslideBarX, 140, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 190, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, RightslideBarX, 240, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 290, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			if (LeftslideBarX == 340) //�ٰ� ���� ���ƿ��� �����̹����� ���ھ� �� ���
			{
				BitBlt(hOffdc, RightslideBarX + 20, 45, 112, 32, hMemdc[3], 1, 1, SRCPAINT);
				BitBlt(hOffdc, RightslideBarX + 20, 95, 128, 32, hMemdc[1], 1, 347, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 145, 128, 32, hMemdc[1], 130, 446, SRCPAINT);
				BitBlt(hOffdc, RightslideBarX + 20, 195, 128, 32, hMemdc[1], 259, 446, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 245, 128, 32, hMemdc[1], 388, 446, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 295, 128, 32, hMemdc[1], 517, 446, SRCPAINT);

				//1���ڸ��� 10�� �ڸ��� ������ �ʾƼ� 0���� ó��
				BitBlt(hOffdc, 116, 500, 219, 80, hMemdc[3], 116, 83, SRCAND); //���ھ� �� �̹���
																			   //����Ʈ ȭ�鿡 ���ھ� ���
				BitBlt(hOffdc, 130, 530, 26, 32, hMemdc[3], 495 + (score / 100000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 160, 530, 26, 32, hMemdc[3], 495 + ((score % 100000) / 10000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 190, 530, 26, 32, hMemdc[3], 495 + ((score % 10000) / 1000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 220, 530, 26, 32, hMemdc[3], 495 + (score % 1000 / 100) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 250, 530, 26, 32, hMemdc[3], 495, 42, SRCPAINT);
				BitBlt(hOffdc, 280, 530, 26, 32, hMemdc[3], 495, 42, SRCPAINT);
				//����Ʈ ȭ�鿡 ���� ���� ���
				{
					BitBlt(hOffdc, 530, 44, 26, 32, hMemdc[3], 495 + (totalNote / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 44, 26, 32, hMemdc[3], 495 + ((totalNote % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 44, 26, 32, hMemdc[3], 495 + ((totalNote % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 44, 26, 32, hMemdc[3], 495 + (totalNote % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 94, 26, 32, hMemdc[3], 495 + (koolCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 94, 26, 32, hMemdc[3], 495 + ((koolCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 94, 26, 32, hMemdc[3], 495 + ((koolCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 94, 26, 32, hMemdc[3], 495 + (koolCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 144, 26, 32, hMemdc[3], 495 + (coolCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 144, 26, 32, hMemdc[3], 495 + ((coolCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 144, 26, 32, hMemdc[3], 495 + ((coolCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 144, 26, 32, hMemdc[3], 495 + (coolCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 194, 26, 32, hMemdc[3], 495 + (goodCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 194, 26, 32, hMemdc[3], 495 + ((goodCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 194, 26, 32, hMemdc[3], 495 + ((goodCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 194, 26, 32, hMemdc[3], 495 + (goodCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 244, 26, 32, hMemdc[3], 495 + (missCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 244, 26, 32, hMemdc[3], 495 + ((missCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 244, 26, 32, hMemdc[3], 495 + ((missCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 244, 26, 32, hMemdc[3], 495 + (missCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 294, 26, 32, hMemdc[3], 495 + (failCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 294, 26, 32, hMemdc[3], 495 + ((failCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 294, 26, 32, hMemdc[3], 495 + ((failCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 294, 26, 32, hMemdc[3], 495 + (failCount % 10) * 27, 42, SRCPAINT);
				}
				//��� ���
				BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 169, 548, SRCCOPY); //56, 68
				MusicNum++;
			}
			if (ESC == PUSH_KEY || ENTER == PUSH_KEY) {
				bgstate = 0;
				noteSpeed = 1;
				lifeGauge = 100;
				g_pChannel[1]->stop();
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[BGM], false, &g_pChannel[0]); //0ä�ο�, 0���� ���(BGM)

				//�ܿ���Ʈ ����
				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt != nList.end())
						nIt = nList.erase(nIt);
					else
						nIt++;
				}
							
				score = 0;
				totalNote = -1;
				koolCount = 0;
				coolCount = 0;
				goodCount = 0;
				missCount = 0;
				failCount = 0;
			}
		}
		else if (bgstate == 3) //�� Ŭ���� �� (���(bgstate == 2�� ����,�Ҹ�,���ܿ� �������� ����))
		{
			RightslideBarX -= 10; //������ ȸ���� �������� �̵�
			LeftslideBarX += 10; //���� ȸ���� ���������� �̵�

			if (RightslideBarX < 340)
				RightslideBarX = 340;
			if (LeftslideBarX > 340)
				LeftslideBarX = 340;
			BitBlt(hdc, 0, 0, 1024, 700, hOffdc, 0, 0, SRCCOPY);
			BitBlt(hOffdc, 0, 0, 1024, 700, hMemdc[2], 0, 0, SRCCOPY);
			BitBlt(hOffdc, 100, 20, 800, 600, hMemdc[5], 0, 0, SRCCOPY);

			//resultâ�� ���ƿ��� ���� ��
			BitBlt(hOffdc, RightslideBarX, 40, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 90, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, RightslideBarX, 140, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 190, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, RightslideBarX, 240, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			BitBlt(hOffdc, LeftslideBarX, 290, 320, 40, hMemdc[3], 468, 1, SRCCOPY);
			if (LeftslideBarX == 340) //�ٰ� ���� ���ƿ��� �����̹����� ���ھ� �� ���
			{
				BitBlt(hOffdc, RightslideBarX + 20, 45, 112, 32, hMemdc[3], 1, 1, SRCPAINT);
				BitBlt(hOffdc, RightslideBarX + 20, 95, 128, 32, hMemdc[1], 1, 347, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 145, 128, 32, hMemdc[1], 130, 446, SRCPAINT);
				BitBlt(hOffdc, RightslideBarX + 20, 195, 128, 32, hMemdc[1], 259, 446, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 245, 128, 32, hMemdc[1], 388, 446, SRCPAINT);
				BitBlt(hOffdc, LeftslideBarX + 20, 295, 128, 32, hMemdc[1], 517, 446, SRCPAINT);

				//1���ڸ��� 10�� �ڸ��� ������ �ʾƼ� 0���� ó��
				BitBlt(hOffdc, 116, 500, 219, 80, hMemdc[3], 116, 83, SRCAND); //���ھ� �� �̹���
																			   //����Ʈ ȭ�鿡 ���ھ� ���
				BitBlt(hOffdc, 130, 530, 26, 32, hMemdc[3], 495 + (score / 100000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 160, 530, 26, 32, hMemdc[3], 495 + ((score % 100000) / 10000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 190, 530, 26, 32, hMemdc[3], 495 + ((score % 10000) / 1000) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 220, 530, 26, 32, hMemdc[3], 495 + (score % 1000 / 100) * 27, 42, SRCPAINT);
				BitBlt(hOffdc, 250, 530, 26, 32, hMemdc[3], 495, 42, SRCPAINT);
				BitBlt(hOffdc, 280, 530, 26, 32, hMemdc[3], 495, 42, SRCPAINT);
				//����Ʈ ȭ�鿡 ���� ���� ���
				{
					BitBlt(hOffdc, 530, 44, 26, 32, hMemdc[3], 495 + (totalNote / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 44, 26, 32, hMemdc[3], 495 + ((totalNote % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 44, 26, 32, hMemdc[3], 495 + ((totalNote % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 44, 26, 32, hMemdc[3], 495 + (totalNote % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 94, 26, 32, hMemdc[3], 495 + (koolCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 94, 26, 32, hMemdc[3], 495 + ((koolCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 94, 26, 32, hMemdc[3], 495 + ((koolCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 94, 26, 32, hMemdc[3], 495 + (koolCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 144, 26, 32, hMemdc[3], 495 + (coolCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 144, 26, 32, hMemdc[3], 495 + ((coolCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 144, 26, 32, hMemdc[3], 495 + ((coolCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 144, 26, 32, hMemdc[3], 495 + (coolCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 194, 26, 32, hMemdc[3], 495 + (goodCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 194, 26, 32, hMemdc[3], 495 + ((goodCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 194, 26, 32, hMemdc[3], 495 + ((goodCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 194, 26, 32, hMemdc[3], 495 + (goodCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 244, 26, 32, hMemdc[3], 495 + (missCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 244, 26, 32, hMemdc[3], 495 + ((missCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 244, 26, 32, hMemdc[3], 495 + ((missCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 244, 26, 32, hMemdc[3], 495 + (missCount % 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 530, 294, 26, 32, hMemdc[3], 495 + (failCount / 1000) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 560, 294, 26, 32, hMemdc[3], 495 + ((failCount % 1000) / 100) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 590, 294, 26, 32, hMemdc[3], 495 + ((failCount % 100) / 10) * 27, 42, SRCPAINT);
					BitBlt(hOffdc, 620, 294, 26, 32, hMemdc[3], 495 + (failCount % 10) * 27, 42, SRCPAINT);
				}
				//��� ���
				if (MusicNum > 65)
					MusicNum = 65;
				if (totalNote * KOOLSCORE == score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 1, 480, SRCCOPY); //s
				else if (totalNote * KOOLSCORE *0.95 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 57, 480, SRCCOPY); //a+
				else if (totalNote * KOOLSCORE *0.9 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 113, 480, SRCCOPY); //a
				else if (totalNote * KOOLSCORE *0.8 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 169, 480, SRCCOPY); //b
				else if (totalNote * KOOLSCORE *0.7 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 1, 548, SRCCOPY); //c
				else if (totalNote * KOOLSCORE *0.6 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 57, 548, SRCCOPY); //d
				else if (totalNote * KOOLSCORE *0.5 < score)
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 113, 548, SRCCOPY); //e
				else
					BitBlt(hOffdc, 370, 510, 55, MusicNum, hMemdc[3], 169, 548, SRCCOPY); //f
				MusicNum++;

				//���⿡ �߰�bestscore ���ű��

			}
			if (ESC == PUSH_KEY || ENTER == PUSH_KEY) {
				totalNote = -1;
				bgstate = 0;
				noteSpeed = 1;
				lifeGauge = 300;
				g_pChannel[1]->stop();
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[BGM], false, &g_pChannel[0]); //0ä�ο�, 0���� ���(BGM)

				for (nIt = nList.begin(); nIt != nList.end();) {
					if (nIt != nList.end())
						nIt = nList.erase(nIt);
					else
						nIt++;
				}
				//���������� DB�� ����
				char query[255];
				mysql_init(&conn);
				connection = mysql_real_connect(&conn, HOST, ID, PASSWD, DBNAME, 3306, NULL, 0);

				if (connection == NULL) //���ῡ ���н�
				{
					exit(1);
				}
				int count = 0;
				int bestScore[10]; //�����ͺ��̽� ���� (� �ְ���) �޾ƿ����

				char buffer[7]; //6�ڸ� ���ھ �Ѱ��ֱ� ����

				query_state = mysql_query(connection, "select score from `rhythm`.`rhythmtable`"); //DB���� score�κи� ������(Score���̺��� score���)
				sql_result = mysql_store_result(connection);

				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					bestScore[count] = atoi(sql_row[0]);
					count++;
				}
				if (score > bestScore[musicSeq])
				{
					sprintf(query, "UPDATE `rhythm`.`rhythmtable` SET `Score` = '%s' WHERE `rhythmtable`.`MusicNum` = '%d'",
						itoa(score, buffer, 10), musicSeq + 1);

					query_state = mysql_query(connection, query);
				}
				sql_result = mysql_store_result(connection);
				if (query_state != 0) //�������� ������ ������
				{
					exit(1);
				}

				score = 0;
				koolCount = 0;
				coolCount = 0;
				goodCount = 0;
				missCount = 0;
				failCount = 0;

				//mysql �����, db��������
				mysql_free_result(sql_result);
				mysql_close(connection);
			}
		}
		//WM_TIMER�ȿ� �����Ƿ� Ÿ�̸� �۵������� ����
		ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONDOWN: // ��� �޴�����
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		//sprintf(buffer, "���콺 x��ǥ : %d, ���콺 y��ǥ : %d", x, y); //563 238
		//TextOut(hOffdc, 0, 0, buffer, 40);
		if (bgstate == 0) {
			if (x >= 563 && x <= 593 && y >= 238 && y <= 268) {//����, �̷� ����
				g_pSystem->playSound(FMOD_CHANNEL_REUSE, g_pSound[OPTION], false, &g_pChannel[1]); //ȿ����
				pattern++;
			}
			if (pattern >= 3)
				pattern = 0;
		}
		break;
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		//��Ʈ��
		for (i = 0; i < 50; i++) {
			DeleteObject(hBitmap[i]);
			DeleteDC(hMemdc[i]);
		}
		//����Ʈ �Ҵ�Ȱ� ����?
		//������ũ��
		DeleteObject(hOffBmp);
		DeleteDC(hOffdc);
		//Ÿ�̸�
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		//�������� ����
		g_pSound[BGM]->release();
		g_pSound[DECIDE]->release();
		g_pSound[OPTION]->release();
		g_pSound[DIAVOLO]->release();
		g_pSound[rbyFAILED]->release();
		g_pSound[rbyCLEAR]->release();
		g_pSound[Freedom_Dive]->release();
		g_pSystem->release(); //�ý��� ����
		g_pSystem->close(); // �ý��� �ݱ�

		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
void Notepattern(int pattern, int MusicNum) { //���߿��� new Note�� Winproc���� ���� �ʿ䰡 �������Ƿ� �ܺη� �� ����
	Note *n = new Note(1);
	Note *n2 = new Note(2);
	Note *n3 = new Note(3);
	Note *n4 = new Note(4);
	Note *n5 = new Note(5);
	Note *n6 = new Note(6);
	Note *endNote = new Note(7);

	FILE *fp; //��Ʈ������ ��� �޸��� �ҷ�����

	enum { NORMAL, MIRROR, RANDOM };
	srand((unsigned)time(NULL)); //rand() �Լ��� ���� ���� �������� �����ϰ� �ϱ�����
	int randnum[6];

	for (int i = 0; i < 6; i++) {
		while (true) {
			bool flag = true;
			randnum[i] = rand() % 6;

			for (int j = 0; j < i; j++) {
				if (randnum[i] == randnum[j])
					flag = false;
			}
			if (flag == true) {
				break;
			}
		}
	}
	switch (MusicNum) {
	case 0:
		fp = fopen("resource\\Diavolo_note.txt", "rt");
		break;
	case 1:
		fp = fopen("resource\\FreedomDive.txt", "rt");
		break;
	}
	int lineCount = 0;
	char str[10];

	if (fp == NULL)
		exit(1);
	while (!feof(fp))
	{
		for (int i = 0; i < 10; i++)
			str[i] = ' ';
		fgets(str, 10, fp); //������ ���� ���پ� �б�
		if (str[0] == 'e')
		{
			endNote->pos.y = PERFECT_JUDGE - lineCount;
			nList.push_back(*endNote);
		}
		switch (pattern) { // 1�� ���������� Ʈ������ ��Ʈ �߰�
		case NORMAL:
			if (str[0] == '1') {
				n->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n);
			}
			if (str[1] == '1') {
				n2->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n2);
			}
			if (str[2] == '1') {
				n3->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n3);
			}
			if (str[3] == '1') {
				n4->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n4);
			}
			if (str[4] == '1') {
				n5->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n5);
			}
			if (str[5] == '1') {
				n6->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n6);
			}
			break;
		case RANDOM:
			if (str[randnum[0]] == '1') {
				n->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n);
			}
			if (str[randnum[1]] == '1') {
				n2->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n2);
			}
			if (str[randnum[2]] == '1') {
				n3->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n3);
			}
			if (str[randnum[3]] == '1') {
				n4->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n4);
			}
			if (str[randnum[4]] == '1') {
				n5->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n5);
			}
			if (str[randnum[5]] == '1') {
				n6->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n6);
			}
			break;
		case MIRROR:
			if (str[0] == '1') {
				n6->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n6);
			}
			if (str[1] == '1') {
				n5->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n5);
			}
			if (str[2] == '1') {
				n4->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n4);
			}
			if (str[3] == '1') {
				n3->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n3);
			}
			if (str[4] == '1') {
				n2->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n2);
			}
			if (str[5] == '1') {
				n->pos.y = PERFECT_JUDGE - lineCount;
				nList.push_back(*n);
			}
			break;
		}
		if (str[7] == 'a') {
			lineCount += 15;
		}
		if (str[7] == 'b') {
			lineCount += 7;
		}
		lineCount++;
	}
	fclose(fp);

	delete n, n2, n3, n4, n5;
}