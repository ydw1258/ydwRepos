#pragma once

#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

class USER_INFO
{
public:
	int index;
	char userID[128];
	char szBuf[BUFSIZE];
	int len;
	Scene userCurScene;
	int roomNum;
};
