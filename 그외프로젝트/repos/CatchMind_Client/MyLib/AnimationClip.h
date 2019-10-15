#pragma once
#include <vector>
#include <Windows.h>

using namespace std;

class BitMap;
class AnimationClip
{
private:
	vector<BitMap*> bitmapVec;

public:
	int getLength() { return bitmapVec.size(); }

public:
	AnimationClip();
	~AnimationClip();

	void AddFrame(BitMap* bitmap);
	void Render(HDC hdc, int x, int y, int frame);
};

