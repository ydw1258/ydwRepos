#pragma comment(lib, "ws2_32")
#include<WinSock2.h>

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	WSACleanup();
	return 0;
}