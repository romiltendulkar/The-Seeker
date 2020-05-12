/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Gear.h
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/10/2019
- End Header --------------------------------------------------------*/

#pragma once


enum class GEAR_TYPE
{
	TYPE_WEAPON,
	TYPE_ARMOR
};

enum class GEAR_RARITY
{
	COMMONGEAR,
	UNCOMMONGEAR,
	RAREGEAR,
	LEGENDARYGEAR,
	ASTRALGEAR
};

class Gear
{
public:
	Gear();
	
	Gear::Gear(GEAR_TYPE type, GEAR_RARITY rare, unsigned int glevel);
	
	virtual ~Gear() {};

	GEAR_TYPE getType();
	GEAR_RARITY getRarity();
	unsigned int getGearLevel();
	void upgradeGearLevel(unsigned int lvl);

private:
	GEAR_TYPE mType;
	GEAR_RARITY mRarity;
	unsigned int mGearLevel;
};