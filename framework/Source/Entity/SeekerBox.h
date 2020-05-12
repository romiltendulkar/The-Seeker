#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;

class SeekerBox : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	SeekerBox();
	virtual ~SeekerBox();
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
	   In this function, we create the UI 
	*/
	void CreateUI();
public:
	unsigned int key;//entity position in the pool
	int val = 0;
	float health;
	Body* pBd;
	Transform* pTr;
	bool player_visible = false;
};
