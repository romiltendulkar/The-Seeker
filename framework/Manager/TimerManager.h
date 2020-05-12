/**
 * @file TimerManager.h
 * @author Ching-Yen Lin.
 * @date 9/01/19
 * @brief To control the FPS
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include "../Source/pch.h"
class TimerManager
{
public:
	TimerManager();
	~TimerManager();

	
	static TimerManager* GetInstance();
	void Destroy();

	/**
	   Gets total time.
	*/
	float getTotalTime()const;

	/**
	   Gets change in frametime
	*/
	float getDeltaTime()const;

	float StartTime();
	float EndTime();
	/**
	   Resets the variables for FPS calculation
	*/
	void Reset();

	/**
	   Starts the frame
	*/
	void Start();

	/**
   Stops the frame
	*/
	void Stop();

	/**
	   FPS calculation
	*/
	void Tick();
private:
	double mSeconds;
	double mDeltaTime;
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;
	bool misStopped;
	static TimerManager* mInstance;

	__int64 mStartTime;
	__int64 mEndTime;
	float mDeltaTime2;
};
#endif

