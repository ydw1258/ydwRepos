#include<windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// LRESULT 메시지 처리에 대한 결과를 나타내는  32 비트 정수형(헝가리언 표기법). 함수원형 선언
LPSTR SamName = "rhythm_game";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR szCmdLine, int iCmdShow)
{
	HWND hwnd;		// 윈도우 핸들
	MSG	msg;		// 메세지
	WNDCLASS wndClass;		// 윈도우 클래스
									// Window의 스타일을 결정
	wndClass.style = CS_HREDRAW | CS_VREDRAW;// CS_ 는  class style의 약자. 
	// 윈도우의 수직 또는 수평크기가 변할 경우, 다시 그린다는 의미.
	wndClass.lpfnWndProc = WndProc;	// 메시지가 발생될 때마다 이 함수가 호출된다.
	wndClass.cbClsExtra = 0; 	// 예약영역. Windows가 내부적으로 사용하며 특수목적에
	wndClass.cbWndExtra = 0; 	// 사용되는 여분의 공간. 사용하지 않는 경우 0로 Set.
	wndClass.hInstance = hInstance;	// 윈도우 클레스를 등록하는 번호. 이 값은 Winmain의 
									// 인수로 전달된 hInstance값을 그대로 대입해 주면  된다.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Icon지정
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// Cursor지정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);// Window의 배경색을 지정
	wndClass.lpszMenuName = NULL;		// Program이 사용할 메뉴를 지정
	wndClass.lpszClassName = SamName;  	// Window클레스이름을 문자열로 지정
	RegisterClass(&wndClass);		// Window 특성을 set(윈도우 클레스등록)
	hwnd = CreateWindow("rhythm_game",	// window class를 지정하는 문자열
		"rhythm_game",	// window의 titlebar에 나타날 문자열
		WS_OVERLAPPEDWINDOW,  // window style(윈도우 형태)
		CW_USEDEFAULT,	// 실행후 화면상의 Window의 X좌표
		CW_USEDEFAULT,	// 실행후 화면상의 Window의 Y좌표
		1060,	// Window의 Width(폭)
		700,	// Window의 Height(높이) //다만들고나면 창크기 수정 815, 638
		NULL,		// 부모윈도우가 있을때 지정
		NULL,		// 윈도우에서 사용할 메뉴의 핸들을 지정
		hInstance,		// 윈도우를 만드는 주체(프로그램 핸들)를 지정
		NULL);		// CREATESTRUCT의 구조체의 번지.
					//Window의 표시
	ShowWindow(hwnd, iCmdShow);	// hwnd는 window핸들러 
								// iCmdShow는 화면에 출력하는 방법을 지정한다
	UpdateWindow(hwnd);		// 사용자 영역

	while (GetMessage(&msg, NULL, 0, 0)) {
		// GetMessage()함수는 MessageQueue(메시지큐)에 있는 메시지를 꺼낸다.
		// 만약 꺼낸 메시지가 WM_QUIT일때 GetMessage()가 FALSE를 리턴한다.
		TranslateMessage(&msg);	// Translate(번역)
		DispatchMessage(&msg);	// Dispatch(전송)
								//TranslateMessage()는 메시지를 적당한 윈도우프로시저에 Dispatch (전송)한다. 
	}
	return msg.wParam;
}