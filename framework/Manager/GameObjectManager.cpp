/**
 * @file GameObjectManager.h
 * @authors Pratyush Gawai
 * @date 9/21/2019
 * @brief This file has methods to update gameobjects.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "GameObjectManager.h"
#include "MemoryManager.h"
#include "TimerManager.h"
#include "../Source/Entity/Entity.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	for (auto z : LGameObject)
		/*delete z;*/
	LGameObject.clear();
}

GameObjectManager * GameObjectManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new GameObjectManager();
	}
	return mInstance;
}

void GameObjectManager::Destroy()
{
	delete mInstance;
}

void GameObjectManager::Update(float rdeltaTime)
{
	unsigned int n = MemoryManager::GetInstance()->current_position;
	for (unsigned int i = 0; i < n; ++i)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->complete)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->Update(rdeltaTime);
		}
	}
}

void GameObjectManager::DeleteObject(int id)
{
	//send object from both lists to graveyard
}

void GameObjectManager::DeleteObject(GameObject *)
{
	//send object from both lists to graveyard
}

void GameObjectManager::LinkObject(int id, GameObject * go)
{
	//make pair of gameobject and id and send to indirection list
}

