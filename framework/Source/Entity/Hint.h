#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;
class Asset;
class Hint : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Hint();
	virtual ~Hint();
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
	int colliding = 0;
	int ownerEnemy;
	Body* pBd;
	Animation* pAn;
	Asset* pAs;
	std::string hstring;
};