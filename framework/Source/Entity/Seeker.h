#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;

class Seeker : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Seeker();
	virtual ~Seeker();
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
	void CreateUI();
public:
	unsigned int key;//entity position in the pool
	int val = 0;
	float health;
	Body* pBd;
	Transform* pTr;
	bool player_visible = false;
};
