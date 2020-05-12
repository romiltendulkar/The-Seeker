#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Asset;

class Right : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Right();
	virtual ~Right();
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
	void HandleEvent(Event *pEventToHandle);

public:
	unsigned int key;//entity position in the pool
	bool mHit = false;
	int mFrameCounter;
	int resetcounter;
	Body* pBd;
	Asset* pAs;


};
