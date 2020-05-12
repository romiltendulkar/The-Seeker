/**
 * @file EventManager.cpp
 * @author Romil Tendulkar
 * @date 10/10/2019
 * @brief This Class interacts with the events that are required for the game.
		It broadcasts event to objects, adds objects as subscribers to events
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "EventManager.h"
#include "..\Source\Events\MoveEvent.h"


EventManager::EventManager()
{
	mTimedEventsQueue = std::vector<Event *>();
	mEventQueue = std::queue<Event*>();
}


EventManager::~EventManager()
{
	for (auto me : mTimedEventsQueue)
	{
		delete me;
	}
	mTimedEventsQueue.clear();
	while (!mEventQueue.empty())
	{
		mEventQueue.pop();
	}
	
}


void EventManager::Update(float rdeltaTime)
{
	std::vector<Event *>::iterator it= mTimedEventsQueue.begin();
	while (it != mTimedEventsQueue.end())
	{
		Event *pE = *it;
		pE->mEventTime -= rdeltaTime;
		if (pE->mEventTime <= 0.0f)
		{
			BroadcastEvent(pE);
			it = mTimedEventsQueue.erase(it);
		}
		else
		{
			++it;
		}
	}
	if (!mEventQueue.empty())
	{
		if (mEventQueue.front()->EventIterations <= 0)
		{
			mEventQueue.pop();
			if (!mEventQueue.empty())
			{
				BroadcastEvent(mEventQueue.front());
				mEventQueue.front()->EventIterations -= rdeltaTime; 
			}
		}
		else
		{
			BroadcastEvent(mEventQueue.front());
			mEventQueue.front()->EventIterations -= rdeltaTime;
		}
	}	
}


void EventManager::BroadcastEvent(Event *pEventToBroadcast)
{
	unsigned int key = MemoryManager::GetInstance()->current_position;
	for (unsigned int i = 0; i < key; ++i)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->HandleEvent(pEventToBroadcast);
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pCr)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[i].pCr->HandleEvent(pEventToBroadcast);
		}
	}
}


void EventManager::BroadcastToSubs(Event *pEventToBroadcast)
{
	for (auto c : mSubscriberLists[pEventToBroadcast->mEventType])
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[c].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[c].pBd->HandleEvent(pEventToBroadcast);
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[c].pCr)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[c].pCr->HandleEvent(pEventToBroadcast);
		}
	}
}


void EventManager::AddTimedEvent(Event *pEventToAdd)
{
	mTimedEventsQueue.push_back(pEventToAdd);
}


void EventManager::Subscribe(EVENT_TYPE type, unsigned int key)
{
	std::unordered_map<EVENT_TYPE, std::vector<unsigned int>>::iterator f = mSubscriberLists.find(type);
	if (f == mSubscriberLists.end())
	{
		std::vector<unsigned int> vec;
		vec.push_back(key);
		mSubscriberLists.insert(std::make_pair(type,vec));
	}
	else
	{
		std::vector<unsigned int> vec = f->second;
		vec.push_back(key);
	}
}

void EventManager::BroadcastToObject(Event* pEventToBroadcast, unsigned int key)
{
	if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->HandleEvent(pEventToBroadcast);
	}
	if ((*MemoryManager::GetInstance()->ObjectPool)[key].pCr)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pCr->HandleEvent(pEventToBroadcast);
	}
	if ((*MemoryManager::GetInstance()->ObjectPool)[key].pEn)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pEn->HandleEvent(pEventToBroadcast);
	}
}

void EventManager::AddQueueEvent(Event* EventToAdd)
{
	mEventQueue.push(EventToAdd);
}