#pragma once

#define BUFSIZE 1024
#define WM_SOCKET (WM_USER+1)

class USER_INFO
{
public:
	int index;
	int roomIndex;
	char userID[128];
	char szBuf[BUFSIZE];
	int len;
	bool isPlaying = false;
};
