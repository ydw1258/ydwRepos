#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include <stdlib.h>
//winmm.lib //이것을 프로젝트 속성 - 명령줄에 추가 하여야 됨
static int fps = 0, fps_c = 0;
static double fps_time_c = 0;

enum {
	FREE_KEY,      // 키가 눌리지 않은 상태
	PUSH_KEY,      // 키를 누른 순간
	PULL_KEY,      // 키가 떼어진 순간
	HOLD_KEY,      // 키가 눌리고 있는 상태
};
void Key_Input(unsigned int Code, int* Flag)
{
	if ((GetKeyState(Code) & 0x80) == 0x80)
	{
		if (*Flag == FREE_KEY) {
			*Flag = PUSH_KEY;
		}
		else {
			*Flag = HOLD_KEY;
		}
	}
	else {
		if (*Flag == PUSH_KEY || *Flag == HOLD_KEY) {
			*Flag = PULL_KEY;
		}
		else {
			*Flag = FREE_KEY;
		}
	}
}
void Fps()
{
	if (fps_time_c < 1000.0F)
	{
		if ((fps_time_c + 1000.0F) < (timeGetTime() + 1000.0F))
		{
			fps_time_c = timeGetTime() + 1000.0f;
			fps = fps_c;
			fps_c = 0;
		}
		fps_c++;
	}
	else
	{
		if (fps_time_c < timeGetTime()) {
			fps_time_c = timeGetTime() + 1000.0f;
			fps = fps_c;
			fps_c = 0;
		}
		else
			fps_c++;
	}
}
static HBITMAP	hBitmap[50], hOffBmp;
static HDC	hMemdc[50], hOffdc;

static int ani = 0, ani_boom[10], ani_kool[10], ani_eff[10];
static int bg_move1, bg_move2, bg_move3;
static int map_ch = 0;
static int enair_x[10] = { 530,530,530,530,530,530,530,530,530,530 },
enair_y[10] = { 120,120,120,120,120,120,120,120,120,120 };

static int wap_x[10] = { -10 }, wap_y[10] = { -10 };
static int enwap_x[10] = { -10 }, enwap_y[10] = { -10 };
static int wap_num, en_move = 0, score = 0, wapsc = 0, boomsc = 0;
static int UP_KEY = FREE_KEY;
static int DOWN_KEY = FREE_KEY;
static int LEFT_KEY = FREE_KEY;
static int RIGHT_KEY = FREE_KEY;
static int SPACE_KEY = FREE_KEY;
static int ESC = FREE_KEY;
static int ENTER = FREE_KEY;
static int S_KEY = FREE_KEY;
static int D_KEY = FREE_KEY;
static int F_KEY = FREE_KEY;
static int J_KEY = FREE_KEY;
static int K_KEY = FREE_KEY;
static int L_KEY = FREE_KEY;

static bool wap[10], on[19], boom[10];
static bool enwap[10], enon[19], enboom[10];
static bool en_life[10], scstop;
char buffer[10], buffer2[50], buffer3[50];
char game[30] = "게임오버!";
char sco[30] = "마우스를 클릭하면 시작됩니다.";
int i, j, ax;
