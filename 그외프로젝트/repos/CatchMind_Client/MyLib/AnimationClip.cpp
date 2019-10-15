#include "AnimationClip.h"
#include "BitMap.h"

AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::AddFrame(BitMap* bitmap)
{
	bitmapVec.push_back(bitmap);
}

void AnimationClip::Render(HDC hdc, int x, int y, int frame)
{
	bitmapVec[frame]->DrawTransparent(hdc, x, y);
}