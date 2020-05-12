#include "../pch.h"
#include "SpotBox.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/TimerManager.h"
//#include "LightList.h"

SpotBox::SpotBox() : Entity(ENTITY_TYPE::SPOTBOX)
{
	mTimer = 0;
	mTimerLimit = 3.0;
}

SpotBox::~SpotBox() {}

void SpotBox::Serialize(Json::Value root, unsigned int position)
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
		pAs->materialID = -1;
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(1, 1, 0);
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
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "SpotBox";
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	this->runtime = false;
	key = position;
	CreateSpotLight();
	++ObjectFactoryManager::GetInstance()->mUDct;
	if (ObjectFactoryManager::GetInstance()->mUDct % 2 == 0)
	{
		mGoingUp = true;
	}
	else
	{
		mGoingUp = false;
	}
}

void SpotBox::Update(float deltaTime)
{
	mTimer += TimerManager::GetInstance()->getDeltaTime();
	if (mTimer > mTimerLimit) {
		mGoingUp = !mGoingUp;
		mTimer = 0;
	}
	if (mGoingUp)
	{
		pTr->mPositionX += 0.1f;
		pBd->mPos_x = pTr->mPositionX;
	}
	else
	{
		pTr->mPositionX -= 0.1f;
		pBd->mPos_x = pTr->mPositionX;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[key+1].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[key+1].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[key+1].pTr->mPositionZ = pTr->mPositionZ + 50.0f;
	
	/*if (lit)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pLd->pLt->SetDiffuse(1, 0, 0);
	}*/
}

void SpotBox::CreateSpotLight()
{
	int curKey = MemoryManager::GetInstance()->current_position;
	++curKey;
	ObjectFactoryManager::GetInstance()->LoadObject("SpotLight",curKey);
}
