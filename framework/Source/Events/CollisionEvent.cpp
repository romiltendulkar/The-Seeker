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

#include "..\pch.h"
#include "CollisionEvent.h"
#include "..\..\Manager\EventManager.h"

CollisionEvent::CollisionEvent(Body* pBody1, Body* pBody2) : Event(EVENT_TYPE::COLLIDE_EVENT, 0)
{
	mpBody1 = pBody1;
	mpBody2 = pBody2;
}

CollisionEvent::~CollisionEvent()
{
	mpBody1 = nullptr;
	mpBody2 = nullptr;
}

void CollisionEvent::CollisionBroadCast()
{
	if (mpBody1 != nullptr)
	{
		if (mpBody2 != nullptr)
		{
			EventManager::GetInstance()->BroadcastToObject(this, mpBody1->key);
			if (mpBody2 != nullptr)
			{
				EventManager::GetInstance()->BroadcastToObject(this, mpBody2->key);
			}
		}
	}
}