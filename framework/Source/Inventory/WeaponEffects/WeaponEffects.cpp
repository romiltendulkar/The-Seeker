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

#include "WeaponEffects.h"


WeaponEffects::WeaponEffects()
{
	mProcChance = 50.0f;
	mDuration = 1.0f;
	mType = WEFFECT_TYPE::WBURN_EFFECT;
	mEffectName = "Burning";
}

WeaponEffects::WeaponEffects(float proc, float duration, WEFFECT_TYPE type)
{
	mProcChance = proc;
	mDuration = duration;
	mType = type;

	switch (mType)
	{
	case WEFFECT_TYPE::WBURN_EFFECT:
		mEffectName = "Burning";
		break;
	case WEFFECT_TYPE::WLIGHTNING_EFFECT:
		mEffectName = "Lightning";
		break;
	}
}

float WeaponEffects::getProcChance()
{
	return mProcChance;
}

float WeaponEffects::getEffectDuration()
{
	return mDuration;
}

WEFFECT_TYPE WeaponEffects::getEffectType()
{
	return mType;
}

std::string WeaponEffects::getEffectName()
{
	return mEffectName;
}

void WeaponEffects::reduceTime(float deltatime)
{
	mDuration -= deltatime;
}