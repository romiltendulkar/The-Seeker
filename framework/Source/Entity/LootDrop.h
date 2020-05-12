/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Gear.h
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/12/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "Entity.h"
#include "..\..\Manager\MemoryManager.h"
#include "..\Inventory\Weapon.h"
#include "..\Inventory\Armor.h"

class Body;
class Transform;

class LootDrop : public Entity
{
public:
	LootDrop();
	LootDrop(float posx, float posy, float scaleX, float scaleY);

	~LootDrop();

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
	
public:
	Body* pBd;
	Transform* pTr;
	Gear* drop;
	bool Looted;
	unsigned int key;
};