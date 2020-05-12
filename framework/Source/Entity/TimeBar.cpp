#include "../pch.h"
#include "TimeBar.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../Entity/Player.h"
#include "../../Manager/GraphicsManager.h"

TimeBar::TimeBar() : Entity(ENTITY_TYPE::TIMEBAR)
{
	xscale = 1;
}

TimeBar::~TimeBar() {}

void TimeBar::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		pTr->mRotationX = 90;
		this->pTr = pTr;
		xscale = pTr->mScaleX;
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->materialID = 3;
		pAs->Serialize(dataRoot, position);
		if ((*MemoryManager::GetInstance()->ObjectPool)[position - 1].pEn->GetEntityType() == ENTITY_TYPE::ENEMY)
		{
			pAs->obj->SetDiffuse(1, 0, 0);
		}
		else
		{
			pAs->obj->SetDiffuse(1, 1, 0);
		}
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
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "TimeBar";
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	this->runtime = false;
	key = position;
}

void TimeBar::Update(float deltaTime)
{
}

void TimeBar::ReScale(float percent)
{
	if (percent / 100 < xscale && prevH < 100)//if percent is less than scale
	{
		pTr->mScaleX -= (percent / 100);
	}

}
