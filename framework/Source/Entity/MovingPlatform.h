#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;

class MovingPlatform : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	MovingPlatform();
	virtual ~MovingPlatform();
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

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Transform* pTr;
	Animation* pAn;
	float speed;
	unsigned int type = 1;
};
