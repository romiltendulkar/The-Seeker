#include "../pch.h"
#include "Switch.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "..\Events\CollisionEvent.h"
#include "Pillar.h"

Switch::Switch() : Entity(ENTITY_TYPE::SWITCH)
{
}

Switch::~Switch() {}

void Switch::Serialize(Json::Value root, unsigned int position)
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
		pAs->obj->SetDiffuse(0,0,0);
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Switch";
	key = position;
	this->complete = true;
	this->runtime = true;
}

void Switch::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	
}

void Switch::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::SWITCH)
			{
				if (b2 == ENTITY_TYPE::BLOCK)
				{
					int k = pEvent->mpBody1->key - 1;
					Pillar* pl = static_cast<Pillar*>((*MemoryManager::GetInstance()->ObjectPool)[k].pEn);
					pl->SetColor();
				}
				
			}
			else if (b2 == ENTITY_TYPE::SWITCH)
			{
				if (b1 == ENTITY_TYPE::BLOCK)
				{
					int k = pEvent->mpBody2->key - 1;
					Pillar* pl = static_cast<Pillar*>((*MemoryManager::GetInstance()->ObjectPool)[k].pEn);
					pl->SetColor();
				}
				
			}
		}
	}
}