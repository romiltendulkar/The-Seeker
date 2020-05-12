/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Weapon.h
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/12/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "WeaponEffects.h"

class LightningW : public WeaponEffects
{
public:
	LightningW();
	LightningW(float dmg, float proc, float time);

	~LightningW();

	std::string getEffectDescription();
	float getLightningDamage();

private:
	float mLightningDamage;
};


