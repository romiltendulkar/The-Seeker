#include "../pch.h"
#include "Right.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/MemoryManager.h"

#include "..\Events\CollisionEvent.h"
#include "Charger.h"
#include "Shooter.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Exploder.h"
#include "..\Events\HitEvent.h"
#include "..\..\Manager\EventManager.h"


Right::Right() : Entity(ENTITY_TYPE::RIGHT)
{
	key = -1;
	mFrameCounter = 0;
	resetcounter = 100;
	pBd = nullptr;
}

Right::~Right() {}

void Right::Serialize(Json::Value root, unsigned int position)
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
		this->pAs = pAs;
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		pBd->active = false;

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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Right";
	key = position;
	this->complete = true;

}

void Right::Update(float deltaTime)
{
	//mFrameCounter++;
	if (mFrameCounter > 0)
	{
		mFrameCounter--;
		/*pAs->tshape->mActive = true;*/
		//std::cout << "Horizontal active" << std::endl;
		//MemoryManager::GetInstance()->FreeAt(key);
	}
	if (mFrameCounter <= 0)
	{
		pBd->active = false;
		mFrameCounter = 0;
		/*pAs->tshape->mActive = false;*/
		//std::cout << "Horizontal inactive" << std::endl;
	}
	if (resetcounter > 0)
	{
		resetcounter--;
	}
	if (resetcounter <= 0)
	{
		resetcounter = 0;

	}
}

void Right::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT && !mHit)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::RIGHT)
			{
				if (b2 == ENTITY_TYPE::CHARGER)
				{
					Charger* pl = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::BOSS1)
				{
					Boss1* pl = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::BOSS2)
				{
					Boss2* pl = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::SHOOTER)
				{
					Shooter* pl = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::EXPLODER)
				{
					Exploder* pl = static_cast<Exploder*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
			}
			else if (b2 == ENTITY_TYPE::RIGHT)
			{
				if (b1 == ENTITY_TYPE::CHARGER)
				{
					Charger* pl = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::BOSS1)
				{
					Boss1* pl = static_cast<Boss1*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::BOSS2)
				{
					Boss2* pl = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::SHOOTER)
				{
					Shooter* pl = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::EXPLODER)
				{
					Exploder* pl = static_cast<Exploder*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					mHit = true;
					//pl->health--;
				}
			}
		}
	}
	
}