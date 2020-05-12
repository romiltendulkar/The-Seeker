#include "../pch.h"
#include "LevelSwitch.h"
#include "../Events/CollisionEvent.h"

LevelSwitch::LevelSwitch() : Entity(ENTITY_TYPE::LEVELSWITCH)
{

}

void LevelSwitch::Serialize(Json::Value root, unsigned int position)
{
	Json::Value dataRoot = root["Transform"];
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	key = position;
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
		pAs->obj->SetDiffuse(0.5f, 0.5f, 0.1f);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}
	dataRoot = root["Level"];

	if (!dataRoot.isNull())
	{
		mLevelName=dataRoot["LevelName"].asString();
	}
	this->complete = true;
}


LevelSwitch::~LevelSwitch()
{

}

void LevelSwitch::Update(float deltaTime)
{

}

void LevelSwitch::HandleEvent(Event * pEventToHandle)
{

	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;
			CollisionEvent *pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();
			if (b2 == ENTITY_TYPE::PLAYER|| b1 == ENTITY_TYPE::PLAYER)
			{
				cout << mLevelName << endl;
				MemoryManager::GetInstance()->mpOwner->ChangeLevel(mLevelName, 1);
			}
		}
	}
}
