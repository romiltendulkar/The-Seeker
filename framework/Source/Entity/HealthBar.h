#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class HealthBar : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	HealthBar();
	virtual ~HealthBar();
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
	   In this function, health bar is rescaled according to current health
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
