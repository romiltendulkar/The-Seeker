#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Transform;
class Door : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Door();
	virtual ~Door();
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
	bool mConditionVariable = false;
	bool  checkonce = false;
public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Animation* pAn;
	Transform* pTr;
	Vector3 CBoxPos;
	Vector3 CBoxScale;
	unsigned int CBoxType;
	unsigned int Type1Counter;
	unsigned int Type2Counter;
	unsigned int Type3Counter;
	unsigned int Enemy_Killed;
	unsigned int Enemy_Required;
	bool w, a, s, d;
};
