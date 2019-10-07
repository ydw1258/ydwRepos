#include<windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// LRESULT �޽��� ó���� ���� ����� ��Ÿ����  32 ��Ʈ ������(�밡���� ǥ���). �Լ����� ����
LPSTR SamName = "rhythm_game";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR szCmdLine, int iCmdShow)
{
	HWND hwnd;		// ������ �ڵ�
	MSG	msg;		// �޼���
	WNDCLASS wndClass;		// ������ Ŭ����
									// Window�� ��Ÿ���� ����
	wndClass.style = CS_HREDRAW | CS_VREDRAW;// CS_ ��  class style�� ����. 
	// �������� ���� �Ǵ� ����ũ�Ⱑ ���� ���, �ٽ� �׸��ٴ� �ǹ�.
	wndClass.lpfnWndProc = WndProc;	// �޽����� �߻��� ������ �� �Լ��� ȣ��ȴ�.
	wndClass.cbClsExtra = 0; 	// ���࿵��. Windows�� ���������� ����ϸ� Ư��������
	wndClass.cbWndExtra = 0; 	// ���Ǵ� ������ ����. ������� �ʴ� ��� 0�� Set.
	wndClass.hInstance = hInstance;	// ������ Ŭ������ ����ϴ� ��ȣ. �� ���� Winmain�� 
									// �μ��� ���޵� hInstance���� �״�� ������ �ָ�  �ȴ�.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Icon����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// Cursor����
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);// Window�� ������ ����
	wndClass.lpszMenuName = NULL;		// Program�� ����� �޴��� ����
	wndClass.lpszClassName = SamName;  	// WindowŬ�����̸��� ���ڿ��� ����
	RegisterClass(&wndClass);		// Window Ư���� set(������ Ŭ�������)
	hwnd = CreateWindow("rhythm_game",	// window class�� �����ϴ� ���ڿ�
		"rhythm_game",	// window�� titlebar�� ��Ÿ�� ���ڿ�
		WS_OVERLAPPEDWINDOW,  // window style(������ ����)
		CW_USEDEFAULT,	// ������ ȭ����� Window�� X��ǥ
		CW_USEDEFAULT,	// ������ ȭ����� Window�� Y��ǥ
		1060,	// Window�� Width(��)
		700,	// Window�� Height(����) //�ٸ������ âũ�� ���� 815, 638
		NULL,		// �θ������찡 ������ ����
		NULL,		// �����쿡�� ����� �޴��� �ڵ��� ����
		hInstance,		// �����츦 ����� ��ü(���α׷� �ڵ�)�� ����
		NULL);		// CREATESTRUCT�� ����ü�� ����.
					//Window�� ǥ��
	ShowWindow(hwnd, iCmdShow);	// hwnd�� window�ڵ鷯 
								// iCmdShow�� ȭ�鿡 ����ϴ� ����� �����Ѵ�
	UpdateWindow(hwnd);		// ����� ����

	while (GetMessage(&msg, NULL, 0, 0)) {
		// GetMessage()�Լ��� MessageQueue(�޽���ť)�� �ִ� �޽����� ������.
		// ���� ���� �޽����� WM_QUIT�϶� GetMessage()�� FALSE�� �����Ѵ�.
		TranslateMessage(&msg);	// Translate(����)
		DispatchMessage(&msg);	// Dispatch(����)
								//TranslateMessage()�� �޽����� ������ ���������ν����� Dispatch (����)�Ѵ�. 
	}
	return msg.wParam;
}