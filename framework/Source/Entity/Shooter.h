#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
#include "..//..//Manager/ObjectFactoryManager.h"

#include "..\Inventory\WeaponEffects\WeaponEffects.h"

class Body;

class Shooter : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Shooter();
	virtual ~Shooter();
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
	void CreateUI();
	float health = 3;
	float ohealth;

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Animation* pAn;
	int frame_counter = 0;
	float player_x, player_y,result_x,result_y;
	bool point = false;
	bool sleep=false;
	WeaponEffects* mAffectedBy;
	WEFFECT_TYPE mWeakness = WEFFECT_TYPE::WLIGHTNING_EFFECT;
};
