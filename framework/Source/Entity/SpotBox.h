#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Transform;
class Animation;

class SpotBox : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	SpotBox();
	virtual ~SpotBox();
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
	//void HandleEvent(Event *pEvent);
	/**
	   In this function, spotlight attached to the spotbox
	*/
	void CreateSpotLight();
public:
	unsigned int key;//entity position in the pool
	int colliding = 0;
	bool lit = false;
	Body* pBd;
	Transform* pTr;
	bool mGoingUp = true;
	float mTimer;
	float mTimerLimit;
};