#include "../pch.h"
#include "Tree.h"


Tree::Tree() : Entity(ENTITY_TYPE::TREE)
{

}


Tree::~Tree()
{
}

void Tree::Serialize(Json::Value root, unsigned int position)
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
	this->complete = true;
}


void Tree::Update(float deltaTime)
{

}

void Tree::HandleEvent(Event * pEventToHandle)
{
}

