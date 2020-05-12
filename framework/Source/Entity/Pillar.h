#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Transform;
class Asset;
class Pillar : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Pillar();
	virtual ~Pillar();
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

	void CreateSwitch();

	int color;

	void SetColor();
	void ResetColor();

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Transform* pTr;
	Asset* pAs;
	int spos;
	bool ran_once = false;
};
