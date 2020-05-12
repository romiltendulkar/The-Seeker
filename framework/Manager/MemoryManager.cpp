/**
 * @file MemoryManager.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 09/27/19
 * @brief This file governs how the Memory Manager is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "MemoryManager.h"
#include "TimerManager.h"
MemoryManager* MemoryManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new MemoryManager();
	}
	return mInstance;
}


void MemoryManager::Destroy()
{
	Deallocate();
	delete mInstance;
}

void MemoryManager::Reallocate()
{
	Deallocate();
	ObjectPool = new std::unordered_map<unsigned int, ObjectNode>;
	Initialize();
	current_position = 0;
	bosscout = 0;
}

void MemoryManager::Initialize()
{
	unsigned int i;
	for (i = 0; i < max_object_number; i++)
	{
		ObjectPool[i];//initializing the map.
	}
}

unsigned int MemoryManager::Allocate()
{
	if (current_position < max_object_number)
		return current_position;
	else
		return -1;
}

void MemoryManager::Deallocate()
{
	//check for memory leak in case of not freeing the allocated elements in the data structure
	for (int i = 0; i < current_position; i++)
	{
		(*ObjectPool)[i].DeleteObjectNode();
		(*ObjectPool).erase(i);
	}
	for (int i = 0; i < mfloor_scale_y; ++i)//left to right
	{
		delete [] MemoryManager::GetInstance()->proxy_map[i];
		delete[] MemoryManager::GetInstance()->map[i];
	}
	delete[] MemoryManager::GetInstance()->proxy_map;
	delete[] MemoryManager::GetInstance()->map;
	delete ObjectPool;
	maincharacher = -1;
	lightList.clear();
}

void MemoryManager::Free()
{
	if (current_position > 0)
		current_position--;
}

void MemoryManager::FreeAt(int key)
{
	(*ObjectPool)[key] = (*ObjectPool)[current_position - 1];
	if (key == maincharacher)
		maincharacher = -1;
	if (lightList.size()>0)
	{
		auto iter = lightList.begin();
		while (iter!= lightList.end())
		{
			if (*iter == key)
			{
				lightList.erase(iter);
				break;
			}
		}
	}
	current_position--;
}
//DEPRECATED : SHIFTED TO GAMEOBJECTMANAGER WHERE IT SHOULD BE
//void MemoryManager::Update() 
//{
//	unsigned int key = current_position;
//	for (unsigned int i = 0; i < key; ++i)
//	{
//		if ((*ObjectPool)[i].pTr)
//		{
//			(*ObjectPool)[i].pTr->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//		if ((*ObjectPool)[i].pCr)
//		{
//			(*ObjectPool)[i].pCr->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//		if ((*ObjectPool)[i].pAn)
//		{
//			(*ObjectPool)[i].pAn->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//	}
//}