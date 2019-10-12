#include <winsock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "..\..\Common\PACKET_HEADER.h"
#include "ServerManager.h"
#pragma comment(lib, "ws2_32")


using namespace std;


int main(int argc, char* argv[])
{
	int retval;
	ServerManager::GetInstance()->Init();

	while (1)
	{
		if (ServerManager::GetInstance()->Update())
			break;
	}
	
	return 0;
}
