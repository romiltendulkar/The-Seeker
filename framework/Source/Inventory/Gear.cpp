/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Weapon.h
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/10/2019
- End Header --------------------------------------------------------*/


#include "..\pch.h"
#include "Gear.h"

Gear::Gear()
{
	mType = GEAR_TYPE::TYPE_WEAPON;
	mRarity = GEAR_RARITY::COMMONGEAR;
	mGearLevel = 1;
}

Gear::Gear(GEAR_TYPE type, GEAR_RARITY rare, unsigned int glevel)
{
	mType = type;
	mRarity = rare;
	mGearLevel = glevel;
}

GEAR_TYPE Gear::getType()
{
	return mType;
}

GEAR_RARITY Gear::getRarity()
{
	return mRarity;
}

unsigned int Gear::getGearLevel()
{
	return mGearLevel;
}

void Gear::upgradeGearLevel(unsigned int lvl)
{
	if (lvl > mGearLevel)
	{
		mGearLevel = lvl;
	}
}