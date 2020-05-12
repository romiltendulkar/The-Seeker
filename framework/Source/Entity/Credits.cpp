/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Credits.cpp
Purpose:
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 04/13/2019
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Credits.h"
#include "../../Manager/MemoryManager.h"
#include "../../Manager/UIManager.h"
Credits::Credits() : Entity(ENTITY_TYPE::CREDITS)
{

}

Credits::~Credits()
{

}

void Credits::Serialize(Json::Value root, unsigned int position)
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
	dataRoot = root["TextureList"];
	mTime = 0;
	if (!dataRoot.isNull())
	{
		for (int i = 0; i < dataRoot.size(); i++)
		{
			auto Textureid2 = ResourceManager::GetInstance()->LoadTexture(dataRoot[i].asString());
			if (Textureid2)
				texturelist.push_back(Textureid2);
		}
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Credits";
	key = position;
	this->complete = true;
	UIManager::GetInstance()->LoadFullScreen(nullptr, 0);
	UIManager::GetInstance()->LoadMainMenu();
	UIManager::GetInstance()->IOShowDialogue(0);
	UIManager::GetInstance()->IOShowHP(0);
	UIManager::GetInstance()->IOShowIngameMenu(0);
	UIManager::GetInstance()->IOShowMain(0);
}

void Credits::Update(float deltaTime)
{
	if (!stop)
	{
		mTime = mTime + deltaTime;
		if (mTime < 2 * (index + 1))
		{
			UIManager::GetInstance()->LoadFullScreen(texturelist[index], 1);
		}
		else
		{
			index++;
			if (index >= texturelist.size())
			{
				stop = 1;
				MemoryManager::GetInstance()->mpOwner->ChangeLevel("MenuLevel", false);
			}
		}
	}
}