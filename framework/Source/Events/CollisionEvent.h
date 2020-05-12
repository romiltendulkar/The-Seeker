/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionEvent.h
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 04/01/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "Event.h"

#include "..\Components\Body.h"

class CollisionEvent : public Event
{
public:
	CollisionEvent(Body* pBody1, Body* pBody2);
	~CollisionEvent();

	void CollisionBroadCast();

public:
	Body* mpBody1;
	Body* mpBody2;
};
