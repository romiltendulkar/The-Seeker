/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MoveEvent.h
Purpose: The class definition for the Move Event. This event can be used to move particular objects.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 20/10/2019
- End Header --------------------------------------------------------*/

#pragma once
#include"Event.h"
#include "..\Components\Controller.h"

enum class MOVEDIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class MoveEvent : public Event
{

public:
	MoveEvent();
	MoveEvent(float speed, float timesrepeated, MOVEDIR orientation);
	~MoveEvent();

	void DoMovement(Controller *pCtr);

private:
	float mXSpeed, mYSpeed, mZSpeed, mZRotate;
	MOVEDIR mOrient;
};
