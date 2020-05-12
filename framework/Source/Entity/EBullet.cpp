#include "../pch.h"
#include "EBullet.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "../Events/CollisionEvent.h"


EBullet::EBullet() : Entity(ENTITY_TYPE::EBULLET)
{
	key = -1;
	pBd = nullptr;
}

EBullet::~EBullet() {}

void EBullet::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(1, 0, 0);
		pAs->obj->SetMaterialID(7);
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
		this->pAn = pAn;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "EBullet";
	key = position;
	this->complete = true;
	this->runtime = true;

}

void EBullet::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
}

void EBullet::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if(pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::EBULLET)
			{
				if (b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::PLATFORM)
				{
					pEvent->mpBody1->mPos_x = 99999;
				}
			}
			else if (b2 == ENTITY_TYPE::EBULLET)
			{
				if (b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::PLATFORM)
				{
					pEvent->mpBody2->mPos_x = 99999;
				}
			}
		}
	}
}
