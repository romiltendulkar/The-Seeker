/**
 * @file AnimationClips.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Updates all animations.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
//#define _HAS_STD_BYTE 0
#include "../../pch.h"
#include "AnimationChannel.h"

using namespace std;
class AnimationClips
{
public:
	AnimationClips();
	AnimationClips(double in_duration, double in_tickspersecond, unsigned int );
	void addChennel(string,AnimationChannel);
	AnimationChannel& getChennel(string);
	void Update(float time, bool isLoop, float speed);
	unordered_map<string, AnimationChannel>& getChennelList();
	float Animationtotaltime;

	~AnimationClips();
private:
	double duration;//in ticks
	double tickspersecond;
	unsigned int numchannels;
	bool isLoop;
	unordered_map<string, AnimationChannel> chennellist;


};

