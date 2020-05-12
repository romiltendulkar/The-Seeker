/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: HitEvent.cpp
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/26/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "Event.h"
#include "..\Entity\Entity.h"
#include "..\Inventory\WeaponEffects\WeaponEffects.h"

class EffectDamage : public Event
{
public:
	EffectDamage();
	~EffectDamage();

	void applyEffect(WeaponEffects *pEffect, Entity *pEntity, float deltatime);
};