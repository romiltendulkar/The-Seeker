/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: DestroyEvent.cpp
Purpose: The class definition for the Destory Event. This event can be used to delete particular objects.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 20/10/2019
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "DestroyEvent.h"

DestroyEvent::DestroyEvent() : Event(EVENT_TYPE::DESTROY_EVENT, 0)
{

}

DestroyEvent::DestroyEvent(float time) : Event(EVENT_TYPE::DESTROY_EVENT, time)
{

}

DestroyEvent::~DestroyEvent()
{

}