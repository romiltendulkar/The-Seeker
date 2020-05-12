
/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Exploder.h
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 03/10/2019
- End Header --------------------------------------------------------*/


#pragma once
#include "Entity.h"
#include "..\Inventory\WeaponEffects\WeaponEffects.h"

class Body;

class Exploder : public Entity
{
public:
	Exploder();
	~Exploder();
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
	void HandleEvent(Event* pEvent);

public:
	unsigned int key;//entity position in the pool
	Body* pBd;
	float oScaleX, oScaleY, oScaleZ;
	float fScaleX, fScaleY, fScaleZ;

	float health = 3;

	WeaponEffects* mAffectedBy;
	WEFFECT_TYPE mWeakness = WEFFECT_TYPE::WFREEZING_EFFECT;
};