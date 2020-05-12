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


enum  WEFFECT_TYPE
{
	WBURN_EFFECT,
	WLIGHTNING_EFFECT,
	WFREEZING_EFFECT
};

class WeaponEffects
{
public:
	WeaponEffects();

	WeaponEffects(float proc, float duration, WEFFECT_TYPE type);

	virtual ~WeaponEffects() {};

	float getProcChance();

	float getEffectDuration();

	WEFFECT_TYPE getEffectType();

	std::string getEffectName();

	virtual std::string getEffectDescription() = 0;

	void reduceTime(float deltaTime);  //only use this to reduce the debuff timer on enemy

private:
	float mProcChance;
	float mDuration;
	WEFFECT_TYPE mType;
	std::string mEffectName;
	
};