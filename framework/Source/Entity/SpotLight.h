#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Transform;
class LightDetail;
class SpotLight : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	SpotLight();
	virtual ~SpotLight();
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

public:
	unsigned int key;//entity position in the pool

	Transform* pTr;
	LightDetail* pLd;
	//Light* pLt;

};
