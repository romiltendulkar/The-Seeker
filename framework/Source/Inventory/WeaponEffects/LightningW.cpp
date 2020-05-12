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

#include "..\..\pch.h"

#include "LightningW.h"

LightningW::LightningW() : 
	WeaponEffects(50.0f, 1.0f, WEFFECT_TYPE::WLIGHTNING_EFFECT)
{
	mLightningDamage = 5.0f;
}

LightningW::LightningW(float dmg, float proc, float time) :
	WeaponEffects(proc, time, WEFFECT_TYPE::WLIGHTNING_EFFECT)
{
	mLightningDamage = dmg;
}

LightningW::~LightningW()
{

}

std::string LightningW::getEffectDescription()
{
	std::string retval = "";
	retval += std::to_string(int(mLightningDamage));
	retval += " Lightning damg over ";
	retval += std::to_string(int(getEffectDuration()));
	retval += " Sec\n";
	/*retval += "Chance = ";
	retval += std::to_string(getProcChance());
	retval += "%";*/
	return retval;
}

float LightningW::getLightningDamage()
{
	return mLightningDamage;
}