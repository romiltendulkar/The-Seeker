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
#include "Armor.h"

Armor::Armor() : Gear(GEAR_TYPE::TYPE_ARMOR, GEAR_RARITY::COMMONGEAR, (unsigned int)1)
{
	mDefense = 5;
	mDodgeChance = 1.0f;
	mArmName = "Leather Armor";
	mAType = ARMOR_TYPE::TYPE_BODY;
	randomizeStats();
}

Armor::Armor(float def, float dodge, std::string name, ARMOR_TYPE atype, GEAR_RARITY rare, unsigned int glevel) :
	Gear(GEAR_TYPE::TYPE_ARMOR, rare, glevel)
{
	mDefense = def;
	mDodgeChance = dodge;
	mArmName = name;
	mAType = atype;
	randomizeStats();
}
Armor::~Armor()
{
	
}

float Armor::getDefense()
{
	return mDefense;
}

float Armor::getDodgeChance()
{
	return mDodgeChance;
}

std::string Armor::getArmorName()
{
	return mArmName;
}

void Armor::upgradeDefense(float def)
{
	if (def > mDefense)
	{
		mDefense = def;
	}
}

void Armor::upgradeDodge(float dodge)
{
	if (dodge > mDodgeChance)
	{
		mDodgeChance = dodge;
	}
}

void Armor::changeName(std::string nm)
{
	mArmName = nm;
}

void Armor::randomizeStats()
{
	int pointstoalot = 10 * getGearLevel();
	srand((unsigned int)time(NULL));
	int iter = rand();
	while (pointstoalot > 0)
	{
		srand((unsigned int)time(NULL));
		int temp = rand();
		temp += iter;
		if (temp % 2 == 0)
		{
			mDefense += 1.0f;
			--pointstoalot;
		}
		else
		{
			mDodgeChance += 0.5f;
			--pointstoalot;
		}
		iter += temp / 8;
	}
}

void Armor::CopyStats(Armor* pOrg)
{
	mDefense = pOrg->mDefense;
	mDodgeChance = pOrg->mDodgeChance;
	mArmName = pOrg->mArmName;
	mAType = pOrg->mAType;
}