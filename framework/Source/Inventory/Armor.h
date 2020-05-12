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

enum class ARMOR_TYPE
{
	TYPE_HELMET,
	TYPE_BODY
};

class Armor : public Gear
{
public:
	Armor();
	Armor(float def, float dodge, std::string name, ARMOR_TYPE atype, GEAR_RARITY rare, unsigned int glevel);
	~Armor();

	float getDefense();
	float getDodgeChance();
	std::string getArmorName();
	void upgradeDefense(float def);
	void upgradeDodge(float dodge);
	void changeName(std::string name);

	void randomizeStats();

	void CopyStats(Armor* pOrg);

private:
	float mDefense;
	float mDodgeChance;
	std::string mArmName;
	ARMOR_TYPE mAType;
};