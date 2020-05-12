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

#include "..\..\pch.h"
#include "FreezingW.h"

FreezingW::FreezingW() : WeaponEffects(50.0f, 1.0f, WEFFECT_TYPE::WFREEZING_EFFECT)
{
	mIceDamage = 5.0f;
}

FreezingW::~FreezingW()
{

}

std::string FreezingW::getEffectDescription()
{
	std::string retval = "";
	retval += std::to_string(int(mIceDamage));
	retval += " Ice damg over ";
	retval += std::to_string(int(getEffectDuration()));
	retval += " Sec\n";
	/*retval += "Chance = ";
	retval += std::to_string(getProcChance());
	retval += "%";*/
	return retval;
}

float FreezingW::getFreezingDamage()
{
	return mIceDamage;
}
