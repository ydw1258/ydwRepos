#pragma once
#include <map>
#include <Windows.h>

using namespace std;

class AnimationClip;
class Animator
{
private:
	map<string, AnimationClip*> clips;
	string curKey;
	bool isPlay = false;
	int playSpeed = 10;

	int counter = 0;
	int frame = 0;

public:
	Animator();
	~Animator();

	void AddClip(string key, AnimationClip* clip, int speed = 10);
	void ChangeClip(string key);
	void Play();
	void ChangeSpeed(int i);
	void Stop();
	void Update();
	void Render(HDC hdc, int x, int y);
	void Release();
};

