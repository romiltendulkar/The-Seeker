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

#pragma once
#include "Gear.h"
#include "WeaponEffects/WeaponEffects.h"

class Weapon : public Gear
{
public:
	Weapon();
	Weapon(float dmg, float crit, std::string name, GEAR_RARITY rare, unsigned int glevel);
	~Weapon();

	float getDamage();
	float getCritChance();
	std::string getWepName();
	void upgradeDamage(float dmg);
	void upgradeCritChance(float crit);
	void changeName(std::string nm);
	void randomizeStats();

	void CopyStats(Weapon* pOrg);

public:
	WeaponEffects* mWeaponPerk;

private:
	float mDamage;
	float mCritChance;
	std::string mWepName;
	
};