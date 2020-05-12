/**
 * @file EventManager.h
 * @author Romil Tendulkar
 * @date 10/10/2019
 * @brief This Class interacts with the events that are required for the game.
		It broadcasts event to objects, adds objects as subscribers to events
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once


#include "..\Source\pch.h"
#include "..\Source\Events\Event.h"
#include "MemoryManager.h"

class EventManager
{
public:
	/**
		Initializes the array of timed events
	*/
	EventManager();

	/**
		Clears all events in the timed events array
	*/
	~EventManager();


	static EventManager* GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new EventManager();
		}
		return mInstance;
	}

	/**
		The update function updates the timer on all the events in the timed events array

		\param rdeltaTime | Time passed since the last frame. This is used to update the time for the event
	*/
	void Update(float rdeltaTime);

	/**
		This function calls the handle event function on all components for all objects

		\param pEventToBroadcast | Pointer to the event that has to be broadcast
	*/
	void BroadcastEvent(Event* EventToBroadcast);

	/**
		This function calls the handle event function for all objects subscribed to the event

		\param pEventToBroadcast | Pointer to the event that has to be broadcast
	*/
	void BroadcastToSubs(Event* EventToBroadcast);

	/**
		This function adds events to the timed events array

		\param pEventToAdd | Pointer to the event that is to be added to the timed events array
	*/
	void AddTimedEvent(Event* EventToAdd);

	/**
		This function adds objects to the subscription list for an event

		\param type | The type of the Event, this is an enum type declared in the events header
		\param key | The key to the hash map containing the object
	*/
	void Subscribe(EVENT_TYPE type, unsigned int key);

	void AddQueueEvent(Event* EventToAdd);


	void BroadcastToObject(Event *EventToBroadcast, unsigned int key);

	//variables
	std::vector<Event *>mTimedEventsQueue;
	std::unordered_map<EVENT_TYPE, std::vector<unsigned int>>mSubscriberLists;
	std::queue<Event*>mEventQueue;
	static EventManager *mInstance;
	
};