#include "../pch.h"
#include "SeekerBox.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/TimerManager.h"
//#include "LightList.h"

SeekerBox::SeekerBox() : Entity(ENTITY_TYPE::SEEKERBOX)
{
}

SeekerBox::~SeekerBox() {}

void SeekerBox::Serialize(Json::Value root, unsigned int position)
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
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "SeekerBox";
	this->runtime = false;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	key = position;
}

void SeekerBox::Update(float deltaTime)
{

}
