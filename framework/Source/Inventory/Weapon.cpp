/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Weapon.cpp
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/10/2019
- End Header --------------------------------------------------------*/


#include "..\pch.h"
#include <ctime>
#include "Weapon.h"
#include "WeaponEffects/LightningW.h"
#include "WeaponEffects/BurningW.h"


Weapon::Weapon() : Gear(GEAR_TYPE::TYPE_WEAPON, GEAR_RARITY::COMMONGEAR, (unsigned int)1)
{
	mDamage = 5;
	//mCritChance = 1.0f;
	mCritChance = 50.0f;
	mWepName = "Wooden Sword";
	randomizeStats();
}

Weapon::Weapon(float dmg, float crit, std::string name, GEAR_RARITY rare, unsigned int glevel) : 
	Gear(GEAR_TYPE::TYPE_WEAPON, rare, glevel)
{
	mDamage = dmg;
	mCritChance = crit;
	mWepName = name;
	randomizeStats();
}

Weapon::~Weapon()
{
	delete mWeaponPerk;
}

float Weapon::getDamage()
{
	return mDamage;
}

float Weapon::getCritChance()
{
	return mCritChance;
}

std::string Weapon::getWepName()
{
	return mWepName;
}

void Weapon::upgradeDamage(float dmg)
{
	if (dmg > mDamage)
	{
		mDamage = dmg;
	}
}

void Weapon::upgradeCritChance(float Crit)
{
	if (Crit > mCritChance)
	{
		mCritChance = Crit;
	}
}

void Weapon::changeName(std::string nm)
{
	mWepName = nm;
}

void Weapon::randomizeStats()
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
			mDamage += 1.0f;
			--pointstoalot;
		}
		else
		{
			if (mCritChance < 100.0f)
			{
				mCritChance += 0.5f;
				--pointstoalot;
			}	
		}
		iter += temp/3;
	}


	mWeaponPerk = new BurningW();

}

void Weapon::CopyStats(Weapon* pOrg)
{
	mDamage = pOrg->mDamage;
	mCritChance = pOrg->mCritChance;
	mWepName = pOrg->mWepName;
}