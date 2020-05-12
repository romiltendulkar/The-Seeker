/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Event.h
Purpose: Header for the Event Class. This defines the events base class which will be used by the manager
of the game. i.e.:- Pause, Play, Menu states.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 10/10/2019
- End Header --------------------------------------------------------*/

#pragma once

enum class EVENT_TYPE
{
	COLLIDE_EVENT,
	MOVE_EVENT,
	DESTROY_EVENT,
	HIT_EVENT,
	EFFECTDAMAGE_EVENT,
	CHANGE_E_EFFECT_EVENT
};

class Event
{
public:
	Event(EVENT_TYPE mtype, float timer) : mEventType(mtype), mEventTime(timer) {};
	virtual ~Event() {};
	EVENT_TYPE mEventType;
	float mEventTime;
	float EventIterations;
	
};