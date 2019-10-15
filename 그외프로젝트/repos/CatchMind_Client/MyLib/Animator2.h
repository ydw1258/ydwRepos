#pragma once
#include <map>
#include <Windows.h>

using namespace std;

class AnimClip;
class Animator2
{
private:
	map<string, AnimClip*> clips;
	string curKey;
	bool isPlay = false;
	float playSpeed = 1;
	float counter = 0;
	int frame = 0;
	bool isLoop = true;

public:
	Animator2();
	~Animator2();

	void AddClip(string key, AnimClip* clip);
	void ChangeClip(string key);
	void ChangeSpeed(float i);
	void Play(bool loop);
	void Stop();
	void Update(float deltaTime);
	void Render(HDC hdc, int x, int y);
	void Release();
};

