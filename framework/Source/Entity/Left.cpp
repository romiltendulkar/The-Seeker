#include "../pch.h"
#include "Left.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "../../Manager/MemoryManager.h"
#include "..\Events\CollisionEvent.h"
#include "Charger.h"
#include "Shooter.h"
#include "Boss1.h"
#include "Boss2.h"
#include "..\Events\HitEvent.h"
#include "..\..\Manager\EventManager.h"
#include "Exploder.h"


Left::Left() : Entity(ENTITY_TYPE::LEFT)
{
	key = -1;
	mFrameCounter = 0;
	pBd = nullptr;
	resetcounter = 36;
}

Left::~Left() {}

void Left::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	if ((*MemoryManager::GetInstance()->ObjectPool)[position].pAn)
	{
		//std::cout << "trash" << std::endl;
	}

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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Left";
	key = position;
	this->complete = true;

}

void Left::Update(float deltaTime)
{
	//mFrameCounter++;
	if (mFrameCounter > 0)
	{
		mFrameCounter--;
		//std::cout << "Vertical active" << std::endl;
		//MemoryManager::GetInstance()->FreeAt(key);
	}
	if (mFrameCounter <= 0)
	{
		mFrameCounter = 0;
		pBd->active = false;
		//std::cout << "Vertical inactive" << std::endl;

	}
	if (resetcounter > 0)
	{
		resetcounter--;
	}
	if (resetcounter <= 0)
	{
		resetcounter = 0;

	}
	if (pBd)
		pBd->Update(deltaTime);
	//std::cout <<"HB: "<< pBd->mPos_x << ", " << pBd->mPos_y << ", " << pBd->mPos_z << std::endl;

}

void Left::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT && !mHit)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();
			if (b1 == ENTITY_TYPE::LEFT)
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
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::BOSS2)
				{
					Boss2* pl = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::SHOOTER)
				{
					Shooter* pl = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type=1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b2 == ENTITY_TYPE::EXPLODER)
				{
					Exploder* pl = static_cast<Exploder*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
			}
			else if (b2 == ENTITY_TYPE::LEFT)
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
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::BOSS2)
				{
					Boss2* pl = static_cast<Boss2*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::SHOOTER)
				{
					Shooter* pl = static_cast<Shooter*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);

					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
				if (b1 == ENTITY_TYPE::EXPLODER)
				{
					Exploder* pl = static_cast<Exploder*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					HitEvent* newHit = new HitEvent();
					newHit->type = 1;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);

					EventManager::GetInstance()->BroadcastToObject(newHit, pl->key);
					mHit = true;
					//pl->health--;
				}
			}
		}
	}
}