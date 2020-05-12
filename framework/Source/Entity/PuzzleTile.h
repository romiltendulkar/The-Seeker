#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Asset;
class PuzzleTile : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	PuzzleTile();
	virtual ~PuzzleTile();
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

	void SetColor(int color);
	void SetCorrectColor();
	void HandleEvent(Event *pEvent);
	int color;//0 = red,1=green, 2 = blue

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Asset* pAs;
	bool solved = false;
};
