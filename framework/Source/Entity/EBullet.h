#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class EBullet : public Entity
{
public:
	EBullet();
	virtual ~EBullet();

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
	Animation* pAn;
	Transform* pTr;
	int framecounter = 0;

};
