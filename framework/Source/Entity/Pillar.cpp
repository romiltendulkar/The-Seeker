#include "../pch.h"
#include "Pillar.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include  "../../Manager/ObjectFactoryManager.h"
Pillar::Pillar() : Entity(ENTITY_TYPE::PILLAR)
{
}

Pillar::~Pillar() {}

void Pillar::Serialize(Json::Value root, unsigned int position)
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
		this->pAs = pAs;
		pAs->obj->SetDiffuse(1,1,1);
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Pillar";
	key = position;
	this->complete = true;
	this->runtime = false;
	CreateSwitch();


}
void Pillar::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x - 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y - 5;

	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pBd->mPos_x - 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pBd->mPos_y -5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = 7;

	if (!ran_once)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = 7;
		ran_once = 1;
	}
}

void Pillar::CreateSwitch()
{
	ObjectFactoryManager::GetInstance()->LoadObject("Switch",key+1);
	spos = key + 1;
}

void Pillar::SetColor()
{
	
	if (color == 0)
		pAs->obj->SetDiffuse(1, 0, 0);
	if (color == 1)
		pAs->obj->SetDiffuse(0, 1, 0);
	if (color == 2)
		pAs->obj->SetDiffuse(0, 0, 1);

}

void Pillar::ResetColor()
{
	pAs->obj->SetDiffuse(1, 1, 1);
}
