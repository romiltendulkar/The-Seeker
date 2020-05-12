#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
#include "..//..//Manager/ObjectFactoryManager.h"
#include "..\Inventory\WeaponEffects\WeaponEffects.h"

class Body;
class Player;
class Boss1 : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Boss1();
	virtual ~Boss1();
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
	Animation* pAn;
	Player* pl;
	Transform* pTr;
	int frame_counter = 0;
	int round_counter = 0;
	int mode = 1;
	int pos = 1;
	float health = 500;
	float angle_proxy = 0.17632698;
	bool bossactive = false;
	WeaponEffects* mAffectedBy;
	WEFFECT_TYPE mWeakness = WEFFECT_TYPE::WLIGHTNING_EFFECT;

};
