/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ChangeEquippedEffect.h
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 03/09/2019
- End Header --------------------------------------------------------*/

#include "..\pch.h"
#include "ChangeEquippedEffect.h"

#include "..\Inventory\WeaponEffects\BurningW.h"
#include "..\Inventory\WeaponEffects\LightningW.h"
#include "..\Inventory\WeaponEffects\FreezingW.h"

ChangeEquippedEffect::ChangeEquippedEffect() : Event(EVENT_TYPE::CHANGE_E_EFFECT_EVENT, 0)
{

}

ChangeEquippedEffect::~ChangeEquippedEffect()
{

}

void ChangeEquippedEffect::ChangePlayerEffect(Player* pl, WEFFECT_TYPE type)
{
	if (!(pl->mEWeapon->mWeaponPerk->getEffectType() == type))
	{
		if (type == WEFFECT_TYPE::WBURN_EFFECT)
		{
			BurningW* pE = new BurningW();
			delete pl->mEWeapon->mWeaponPerk;
			pl->mEWeapon->mWeaponPerk = pE;
		}
		if (type == WEFFECT_TYPE::WLIGHTNING_EFFECT)
		{
			LightningW *pE = new LightningW();
			delete pl->mEWeapon->mWeaponPerk;
			pl->mEWeapon->mWeaponPerk = pE;
		}
		if (type == WEFFECT_TYPE::WFREEZING_EFFECT)
		{
			FreezingW* pE = new FreezingW();
			delete pl->mEWeapon->mWeaponPerk;
			pl->mEWeapon->mWeaponPerk = pE;
		}
	}

}