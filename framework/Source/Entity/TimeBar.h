#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class TimeBar : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	TimeBar();
	virtual ~TimeBar();
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
	/**
	   In this function, time bar is rescaled as the time runs out
	*/
	void ReScale(float percent);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool
	float xscale;
	float prevH;

	Body* pBd;
	Transform* pTr;
};
