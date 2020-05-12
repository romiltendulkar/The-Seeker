/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: HitEvent.cpp
Purpose: This is the class used for the freezing effect on weapons.
	This effect will slow down enemies.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/26/2019
- End Header --------------------------------------------------------*/

#include "..\pch.h"
#include "EffectDamage.h"

#include "..\Entity\Boss1.h"
#include "..\Entity\Charger.h"
#include "..\Entity\Shooter.h"

#include "..\Inventory\WeaponEffects\BurningW.h"
#include "..\Inventory\WeaponEffects\LightningW.h"
#include "..\Inventory\WeaponEffects\FreezingW.h"

EffectDamage::EffectDamage() : Event(EVENT_TYPE::EFFECTDAMAGE_EVENT, 0)
{

}

EffectDamage::~EffectDamage()
{

}

void EffectDamage::applyEffect(WeaponEffects* pEffect, Entity* pEntity, float deltatime)
{
	if (pEntity)
	{
		if (pEffect)
		{

			//std::cout << "\n Effect Triggered";
			if (pEffect->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
			{
				BurningW* B = static_cast<BurningW*>(pEffect);
				if (pEntity->mEntityType == ENTITY_TYPE::CHARGER)
				{
					Charger* p = static_cast<Charger*>(pEntity);
					p->health -= B->getBurnDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WBURN_EFFECT)
					{
						p->health -= B->getBurnDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::BOSS1)
				{
					Boss1* p = static_cast<Boss1*>(pEntity);
					p->health -= B->getBurnDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WBURN_EFFECT)
					{
						p->health -= B->getBurnDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::SHOOTER)
				{
					Shooter* p = static_cast<Shooter*>(pEntity);
					p->health -= B->getBurnDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WBURN_EFFECT)
					{
						p->health -= B->getBurnDamage() * deltatime;
					}
				}
			}
			if (pEffect->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
			{
				LightningW* B = static_cast<LightningW*>(pEffect);
				if (pEntity->mEntityType == ENTITY_TYPE::CHARGER)
				{
					Charger* p = static_cast<Charger*>(pEntity);
					p->health -= B->getLightningDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getLightningDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::BOSS1)
				{
					Boss1* p = static_cast<Boss1*>(pEntity);
					p->health -= B->getLightningDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getLightningDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::SHOOTER)
				{
					Shooter* p = static_cast<Shooter*>(pEntity);
					p->health -= B->getLightningDamage() *deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getLightningDamage() * deltatime;
					}
				}
			}
			if (pEffect->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
			{
				FreezingW* B = static_cast<FreezingW*>(pEffect);
				if (pEntity->mEntityType == ENTITY_TYPE::CHARGER)
				{
					Charger* p = static_cast<Charger*>(pEntity);
					p->health -= B->getFreezingDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getFreezingDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::BOSS1)
				{
					Boss1* p = static_cast<Boss1*>(pEntity);
					p->health -= B->getFreezingDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getFreezingDamage() * deltatime;
					}
				}
				else if (pEntity->mEntityType == ENTITY_TYPE::SHOOTER)
				{
					Shooter* p = static_cast<Shooter*>(pEntity);
					p->health -= B->getFreezingDamage() * deltatime;
					if (p->mWeakness == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						p->health -= B->getFreezingDamage() * deltatime;
					}
				}
			}
		}
	}
}