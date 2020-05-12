/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MoveEvent.cpp
Purpose: The class implementation of the Move Event. This event can be used to move particular objects.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 20/10/2019
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "MoveEvent.h"

#include "..\Components\Transform.h"
#include "..\Components\Body.h"
#include "..\Components\Animation.h"

#include "..\..\Manager\MemoryManager.h"

MoveEvent::MoveEvent() : Event(EVENT_TYPE::MOVE_EVENT, 0)
{
	EventIterations = 120;
	mXSpeed = -2;
	mYSpeed = 0;
	mZSpeed = 0;
	mZRotate = -90;
	mOrient = MOVEDIR::LEFT;
}

MoveEvent::MoveEvent(float speed, float timesrepeated, MOVEDIR orientation) :
	Event(EVENT_TYPE::MOVE_EVENT, 0)
{
	EventIterations = timesrepeated;
	mOrient = orientation;
	
	switch (orientation)
	{
		case MOVEDIR::UP:
			mXSpeed = 0.0f;
			mYSpeed = speed;
			mZSpeed = 0.0f;
			mZRotate = 180;
			break;
		case MOVEDIR::DOWN:
			mXSpeed = 0.0f;
			mYSpeed = -speed;
			mZSpeed = 0.0f;
			mZRotate = 0;
			break;
		case MOVEDIR::LEFT:
			mXSpeed = -speed;
			mYSpeed = 0.0f;
			mZSpeed = 0.0f;
			mZRotate = -90;
			break;
		case MOVEDIR::RIGHT:
			mXSpeed = speed;
			mYSpeed = 0.0f;
			mZSpeed = 0.0f;
			mZRotate = 90;
			break;
	}
	
	
}

MoveEvent::~MoveEvent()
{

}

void MoveEvent::DoMovement(Controller *pCtr)
{
	unsigned int key = pCtr->key;
	
	Body *pBd = (*MemoryManager::GetInstance()->ObjectPool)[key].pBd;
	Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
	Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[key].pAn;

	switch (mOrient)
	{
	case MOVEDIR::UP:
		pBd->mVelo_y = mYSpeed;

		pBd->mFacingUp = true;
		pBd->mFacingDown = false;
		pBd->mFacingRight = false;
		pBd->mFacingLeft = false;

		pTr->mRotationZ = mZRotate;
		break;
	case MOVEDIR::DOWN:
		pBd->mVelo_y = mYSpeed;

		pBd->mFacingUp = false;
		pBd->mFacingDown = true;
		pBd->mFacingRight = false;
		pBd->mFacingLeft = false;

		pTr->mRotationZ = mZRotate;
		break;
	case MOVEDIR::LEFT:
		pBd->mVelo_x = mXSpeed;

		pBd->mFacingUp = false;
		pBd->mFacingDown = false;
		pBd->mFacingRight = false;
		pBd->mFacingLeft = true;
		pTr->mRotationZ = mZRotate;
		break;
	case MOVEDIR::RIGHT:
		pBd->mVelo_x = mXSpeed;

		pBd->mFacingUp = false;
		pBd->mFacingDown = false;
		pBd->mFacingRight = true;
		pBd->mFacingLeft = false;

		pTr->mRotationZ = mZRotate;
		break;
	}
	pAn->ChangeCurrentStatus("run", 1, 19 / 30.0f);
}