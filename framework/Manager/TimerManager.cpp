/**
 * @file TimerManager.h
 * @author Ching-Yen Lin.
 * @date 9/01/19
 * @brief To control the FPS
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "TimerManager.h"


TimerManager::TimerManager()
	:mSeconds(0),mDeltaTime(-1.0),mBaseTime(0),
	mPausedTime(0), mPrevTime(0),mCurrTime(0),
	misStopped(false)
{
	__int64 coutsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&coutsPerSec);
	mSeconds = 1.0 / (double)coutsPerSec;

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mStartTime = currTime;

}

TimerManager::~TimerManager()
{
}

TimerManager * TimerManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new TimerManager();
	}
	return mInstance;
}

void TimerManager::Destroy()
{
	delete mInstance;
}

float TimerManager::getTotalTime() const
{
	if (misStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSeconds);
	}
	return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSeconds);
}

float TimerManager::getDeltaTime() const
{
	if (mDeltaTime < 0)
	{
		return 0.0f;
	}
	return (float)mDeltaTime;
}

float TimerManager::StartTime()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mStartTime = currTime;
	return mStartTime;
}
float TimerManager::EndTime()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mEndTime = currTime;

	while ((mEndTime - mStartTime)*mSeconds < 1.0f/60.0f)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mEndTime = currTime;
	}
	mDeltaTime = (mEndTime - mStartTime)*mSeconds;

	return mDeltaTime;
}

void TimerManager::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	misStopped = false;
}

void TimerManager::Start()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	if (misStopped)
	{
		mPausedTime += (currTime - mStopTime);
		mPrevTime = currTime;
		mStopTime = 0;
		misStopped = false;
	}
}

void TimerManager::Stop()
{
	if (!misStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = currTime;
		misStopped = true;
	}
}

void TimerManager::Tick()
{
	if (misStopped)
	{
		mDeltaTime = 0.0;
		return;
	}
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime)*mSeconds;

	mPrevTime = mCurrTime;
	if (mDeltaTime < 0)
	{
		mDeltaTime = 0.0;
	}

}
