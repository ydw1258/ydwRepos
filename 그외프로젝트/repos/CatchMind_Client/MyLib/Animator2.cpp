#include "Animator2.h"
#include "AnimClip.h"

Animator2::Animator2()
{
}

Animator2::~Animator2()
{
}

void Animator2::AddClip(string key, AnimClip * clip)
{
	clips.insert(make_pair(key, clip));
}

void Animator2::ChangeClip(string key)
{
	if (!curKey.compare(key))
		return;

	curKey = key;
	Play(isLoop);
}

void Animator2::Play(bool loop)
{
	isLoop = loop;
	isPlay = true;
	frame = clips[curKey]->getStart();
}

void Animator2::ChangeSpeed(float i)
{
	playSpeed = i;
}

void Animator2::Stop()
{
	isPlay = false;
}

void Animator2::Update(float deltaTime)
{
	if (!isPlay)
		return;

	if (clips.find(curKey) == clips.end())
		return;

	counter += deltaTime;

	if (counter >= 1/playSpeed)
	{
		counter = 0;
		frame++;

		if (frame > clips[curKey]->getEnd())
		{
			if (isLoop)
			{
				frame = clips[curKey]->getStart();
			}
			else
			{
				Stop();
			}
		}
	}
}

void Animator2::Render(HDC hdc, int x, int y)
{
	if (clips.find(curKey) != clips.end())
		clips[curKey]->Render(hdc, x, y, frame);
}

void Animator2::Release()
{
	for (auto iter = clips.begin(); iter != clips.end();)
	{
		delete (*iter).second;
		iter = clips.erase(iter);
	}
	clips.clear();
}

