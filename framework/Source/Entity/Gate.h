#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Transform;
class Gate : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Gate();
	virtual ~Gate();
	/**
	Serializes asset
	Root : JSON Root
	key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int position);
	/**
	   In this function, every component of the said entity is updated.

	  \param rdeltaTime | Delta time between frames.
	*/
	void Update(float deltaTime);
	void HandleEvent(Event *pEvent);
	bool mGoingUp = true;
	float mTimer;
	float mTimerLimit;
public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Animation* pAn;
	Transform* pTr;

};
