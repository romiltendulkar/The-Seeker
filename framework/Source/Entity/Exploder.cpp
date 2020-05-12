/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Exploder.h
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 03/10/2019
- End Header --------------------------------------------------------*/

#include "..\pch.h"
#include "Exploder.h"

#include "..\Components\Body.h"
#include "..\Components\Body.h"
#include "..\Components\Asset.h"
#include "..\Components\Animation.h"

#include "..\..\Manager\MemoryManager.h"
#include "..\Events\HitEvent.h"
#include "..\Events\EffectDamage.h"

#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\EventManager.h"
#include "Player.h"

Exploder::Exploder() : Entity(ENTITY_TYPE::EXPLODER)
{
	mAffectedBy = nullptr;
}

Exploder::~Exploder()
{

}

void Exploder::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(0.0f, 0.0f, 1.0f);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Exploder";
	key = position;
	this->complete = true;
	oScaleX = pBd->mScale_x;
	oScaleY = pBd->mScale_y;
	oScaleZ = pBd->mScale_z;

	fScaleX = pBd->mScale_x + 3;
	fScaleY = pBd->mScale_y + 3;
	fScaleZ = pBd->mScale_z + 3;

}

void Exploder::Update(float deltaTime)
{
	if (health > 0)
	{
		if (pBd->mScale_x < fScaleX)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX += deltaTime;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY += deltaTime;
			pBd->mScale_x += deltaTime;
			pBd->mScale_y += deltaTime;
		}
		else
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX = oScaleX;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY = oScaleY;
			pBd->mScale_x = oScaleX;
			pBd->mScale_y = oScaleY;
		}
		if (mAffectedBy)
		{
			EffectDamage* pED = new EffectDamage();
			pED->applyEffect(mAffectedBy, this, deltaTime);
			delete pED;
			mAffectedBy->reduceTime(deltaTime);
			if (mAffectedBy->getEffectDuration() <= 0)
			{
				delete mAffectedBy;
				mAffectedBy = nullptr;
			}
		}
	}
	else
	{
		pBd->mPos_z = -500;
	}
	
}

void Exploder::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::HIT_EVENT)
		{
			HitEvent* pHit = static_cast<HitEvent*>(pEventToHandle);
			pHit->PerformEnemyDamage(this);
			delete pEventToHandle;
		}
		else if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::EXPLODER)
			{
				if (b2 == ENTITY_TYPE::PLAYER)
				{
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

					if (pBd->mScale_x >= fScaleX)
					{
						HitEvent* newHit = new HitEvent();
						newHit->explosion = true;
						EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
						delete newHit;
					}
				}
			}
			else if (b2 == ENTITY_TYPE::EXPLODER)
			{
				if (b1 == ENTITY_TYPE::PLAYER)
				{
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

					if (pBd->mScale_x >= fScaleX)
					{
						HitEvent* newHit = new HitEvent();
						newHit->explosion = true;
						EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
						delete newHit;
					}
				}
			}
		}
	}
	
}