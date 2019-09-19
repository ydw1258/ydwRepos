#include<Windows.h>
#include<stdio.h>

#define BUFSIZE 10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int k = 1; k <= 500; k++)
	{
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0)
			break;

		for (int i = 0; i < BUFSIZE; i++)
		{
			buf[i] = k;
		}

		SetEvent(hWriteEvent);
	}
	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while (1)
	{
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT_0)
			break;
		printf("Thread : %4d : ", GetCurrentThreadId());

		for (int i = 0; i < BUFSIZE; i++)
			printf("%3d ", buf[i]);
		printf("\n");
		ZeroMemory(buf, sizeof(buf));

		SetEvent(hReadEvent);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
		return 1;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL)
		return 1;

	HANDLE hThread[3];

	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}