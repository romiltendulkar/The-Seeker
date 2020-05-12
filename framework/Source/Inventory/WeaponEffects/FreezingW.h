/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FreezingW.h
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/21/2019
- End Header --------------------------------------------------------*/

#pragma once
#include "WeaponEffects.h"

class FreezingW : public WeaponEffects
{
public:
	FreezingW();
	~FreezingW();
	std::string getEffectDescription();
	float getFreezingDamage();

private:
	float mIceDamage;
};