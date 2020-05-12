/**
 * @file AnimationClips.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Updates all animations.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../../pch.h"
#include "AnimationClips.h"

AnimationClips::AnimationClips()
{
	Animationtotaltime = 0.0f;
	duration = 0.0f;
	tickspersecond = 0.0f;
	numchannels = 0;
	isLoop = false;
}

AnimationClips::AnimationClips(double in_duration, double in_tickspersecond, unsigned int in_numchannels)
	:Animationtotaltime((float)(in_duration/ in_tickspersecond)), duration(in_duration), tickspersecond(in_tickspersecond), numchannels(in_numchannels)
{
}
/**
	Adds channel
*/
void AnimationClips::addChennel(string name, AnimationChannel a)
{
	chennellist[name] = a;
}
/**
	Gets Channel
*/
AnimationChannel & AnimationClips::getChennel(string name)
{
	return chennellist[name];
}

void AnimationClips::Update(float time,bool isLoop,float speed)
{
	float runTime = time;
	float timeintick = runTime * (float)tickspersecond*speed;
	for (auto& i: chennellist)
	{
		i.second.Interpolate(timeintick);
	}
}
/**
	Gets Channel list
*/
unordered_map<string, AnimationChannel>& AnimationClips::getChennelList()
{
	return chennellist;
}


AnimationClips::~AnimationClips()
{
}
