#include "../pch.h"
#include "Hint.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\UIManager.h"

Hint::Hint() : Entity(ENTITY_TYPE::HINT)
{

}

Hint::~Hint() {}

void Hint::Serialize(Json::Value root, unsigned int position)
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
		pAs->materialID = -1;
		pAs->Serialize(dataRoot, position);
		this->pAs = pAs;
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

	dataRoot = root["HintStr"];
	hstring = dataRoot.asString();

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Hint";
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	this->runtime = false;
	key = position;
}

void Hint::Update(float deltaTime)
{
	
}

void Hint::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::HINT)
			{
				if (b2 == ENTITY_TYPE::PLAYER)
				{
					UIManager::GetInstance()->SetDialogue(hstring);
				}
			}
			else if(b2 == ENTITY_TYPE::HINT)
			{
				if (b1 == ENTITY_TYPE::PLAYER)
				{
					UIManager::GetInstance()->SetDialogue(hstring);
				}
			}
		}
	}
}
