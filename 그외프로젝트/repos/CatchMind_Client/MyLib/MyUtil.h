// 프로젝트 설정>c c++>명령중: /await /std:c++latest

#pragma once
#include <Windows.h>
//#include <algorithm>
//using namespace std;

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define	ARRAY_SIZE(x)	( sizeof((x))/sizeof((x)[0]) ) // 배열 길이

namespace MyUtil
{
	void DrawRectCenter(HDC hdc, int x, int y, int width, int height)
	{
		Rectangle(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
	}

	int IndexD2toD1(int x, int y, int sizeX)
	{
		return sizeX * y + x;
	}

	template<typename T>
	T clamp(T value, T min, T max)
	{
		T temp = value;
		if (temp > max) temp = max;
		if (temp < min) temp = min;

		return temp;
	}

	wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
}