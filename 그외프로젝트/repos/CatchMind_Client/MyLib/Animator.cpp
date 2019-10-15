#include "Animator.h"
#include "AnimationClip.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::AddClip(string key, AnimationClip* clip, int speed)
{
	clips.insert(make_pair(key, clip));
}

void Animator::ChangeClip(string key)
{
	if (!curKey.compare(key))
		return;

	curKey = key;
	Play();
}

void Animator::Play()
{
	isPlay = true;
	frame = 0;
}

void Animator::ChangeSpeed(int i)
{
	playSpeed = i;
}

void Animator::Stop()
{
	isPlay = false;
}

void Animator::Update()
{
	if (!isPlay)
		return;

	counter += playSpeed;
	if (counter >= 100)
	{
		counter = 0;
		frame++;

		if (clips.find(curKey) != clips.end())
		{
			if (frame >= clips[curKey]->getLength())
				frame = 0;
		}
	}
}

void Animator::Render(HDC hdc, int x, int y)
{
	if (clips.find(curKey) != clips.end())
		clips[curKey]->Render(hdc, x, y, frame);
}

void Animator::Release()
{
	for (auto iter = clips.begin(); iter != clips.end();)
	{
		delete (*iter).second;
		iter = clips.erase(iter);
	}
	clips.clear();
}
