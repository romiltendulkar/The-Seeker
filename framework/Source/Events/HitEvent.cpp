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
#include "HitEvent.h"
#include "..\..\Manager\MemoryManager.h"
#include "..\..\Manager\DXInputManager.h"
#include "..\Entity\Player.h"
#include "..\Entity\Boss1.h"
#include "..\Entity\Boss2.h"
#include "..\Entity\Charger.h"
#include "..\Entity\Shooter.h"
#include "..\Entity\Exploder.h"
#include "..\..\Manager\ParticleManager.h"
#include "..\Inventory\WeaponEffects\BurningW.h"
#include "..\Inventory\WeaponEffects\LightningW.h"
#include "..\Inventory\WeaponEffects\FreezingW.h"


HitEvent::HitEvent() : Event(EVENT_TYPE::HIT_EVENT, 0)
{

}

HitEvent::~HitEvent()
{

}

void HitEvent::PerformEnemyDamage(Entity* pEntity)
{
	std::cout << counter << std::endl;
	if (counter == 0)
	{
		if (pEntity)
		{
			Player* play;
			if ((*MemoryManager::GetInstance()->ObjectPool)[2].pEn->mEntityType == ENTITY_TYPE::PLAYER)
			{
				play = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
			}
			else
			{
				return;
			}
			if (pEntity->mEntityType == ENTITY_TYPE::CHARGER)
			{
				Charger* p = static_cast<Charger*>(pEntity);

				float player_x = p->pBd->mPos_x;
				float player_y = p->pBd->mPos_y;
				float result_x = player_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
				float result_y = player_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
				float newRot = atan2(result_y, result_x);
				/*if (!(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->pressed_left && p->got_hit_counter == 0)
				{
					p->pBd->mVelo_x = 2 * cos(newRot);
					p->pBd->mVelo_y = 2 * sin(newRot);
					p->got_hit_counter = 15;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
				}
				else if ((*MemoryManager::GetInstance()->ObjectPool)[2].pCr->pressed_left && p->got_hit_counter == 0)
				{
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					p->pBd->mVelo_x = 2 * cos(newRot);
					p->pBd->mVelo_y = 2 * sin(newRot);
					p->got_hit_counter = 2;
				}*/

				p->health -= 70;
				if (play->mOnePunchMan)
				{
					p->health = 0;
				}
				srand(time(NULL));
				int iter = rand() % 100;
				if (iter < play->mEWeapon->getCritChance())
				{
					p->health -= play->mEWeapon->getDamage();
					//std::cout << "\n Crit";
				}
				srand(iter + (iter / 6));
				iter = rand() % 100;
				if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
				{
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
					{
						BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
					{
						FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
				}
			}
			else if (pEntity->mEntityType == ENTITY_TYPE::BOSS1)
			{
				Boss1* p = static_cast<Boss1*>(pEntity);
				p->health -= 2;
				if (play->mOnePunchMan)
				{
					p->health = 0;
				}
				srand(time(NULL));
				int iter = rand() % 100;
				if (iter < play->mEWeapon->getCritChance())
				{
					p->health -= play->mEWeapon->getDamage();
					//std::cout << "\n Crit";
				}
				srand(iter + (iter / 6));
				iter = rand() % 100;
				if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
				{
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
					{
						BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
					{
						FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
				}
				DamageEffect(p->pBd, type);
			}
			else if (pEntity->mEntityType == ENTITY_TYPE::BOSS2)
			{
				Boss2* p = static_cast<Boss2*>(pEntity);
				p->health -= 20;
				srand(time(NULL));
				int iter = rand() % 100;
				if (iter < play->mEWeapon->getCritChance())
				{
					p->health -= play->mEWeapon->getDamage();
					//std::cout << "\n Crit";
				}
				srand(iter + (iter / 6));
				iter = rand() % 100;
				if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
				{
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
					{
						BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
					{
						FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
				}
				DamageEffect(p->pBd, type);
			}
			else if (pEntity->mEntityType == ENTITY_TYPE::SHOOTER)
			{
				Shooter* p = static_cast<Shooter*>(pEntity);
				p->health -= 70;
				if (play->mOnePunchMan)
				{
					p->health = 0;
				}
				srand((unsigned int)time(NULL));
				int iter = rand() % 100;
				if (iter < play->mEWeapon->getCritChance())
				{
					p->health -= play->mEWeapon->getDamage();
					//std::cout << "\n Crit";
				}
				srand(iter + (iter / 6));
				iter = rand() % 100;
				if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
				{
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
					{
						BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
					{
						FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
				}
				DamageEffect(p->pBd, type);
			}
			else if (pEntity->mEntityType == ENTITY_TYPE::EXPLODER)
			{
				Exploder* p = static_cast<Exploder*>(pEntity);
				p->health -= play->mEWeapon->getDamage();
				if (play->mOnePunchMan)
				{
					p->health = 0;
				}
				srand(time(NULL));
				int iter = rand() % 100;
				if (iter < play->mEWeapon->getCritChance())
				{
					p->health -= play->mEWeapon->getDamage();
					//std::cout << "\n Crit";
				}
				srand(iter + (iter / 6));
				iter = rand() % 100;
				if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
				{
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
					{
						BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
					{
						LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
					if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
					{
						FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
						p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
					}
				}
			if (play->mOnePunchMan)
			{
				p->health = 0;
			}
			p->health -= play->mEWeapon->getDamage();
			srand(time(NULL));
			iter = rand() % 100;
			if (iter < play->mEWeapon->getCritChance())
			{
				p->health -= play->mEWeapon->getDamage();
				std::cout << "\n Crit";
			}
			srand(iter + (iter / 6));
			iter = rand() % 100;
			if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
			{
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
				{
					BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
				{
					LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
				{
					FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
			}
			DamageEffect(p->pBd, type);
		}
		else if (pEntity->mEntityType == ENTITY_TYPE::BOSS1)
		{
			Boss1* p = static_cast<Boss1*>(pEntity);
			if (play->mOnePunchMan)
			{
				p->health = 0;
			}
			p->health -= play->mEWeapon->getDamage();
			srand(time(NULL));
			int iter = rand() % 100;
			if (iter < play->mEWeapon->getCritChance())
			{
				p->health -= play->mEWeapon->getDamage();
				std::cout << "\n Crit";
			}
			srand(iter + (iter / 6));
			iter = rand() % 100;
			if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
			{
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
				{
					BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
				{
					LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
				{
					FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
			}
			DamageEffect(p->pBd, type);

		}
		else if (pEntity->mEntityType == ENTITY_TYPE::SHOOTER)
		{
			Shooter* p = static_cast<Shooter*>(pEntity);
			if (play->mOnePunchMan)
			{
				p->health = 0;
			}
			p->health -= play->mEWeapon->getDamage();
			srand((unsigned int)time(NULL));
			int iter = rand() % 100;
			if (iter < play->mEWeapon->getCritChance())
			{
				p->health -= play->mEWeapon->getDamage();
				std::cout << "\n Crit";
			}
			srand(iter + (iter / 6));
			iter = rand() % 100;
			if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
			{
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
				{
					BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
				{
					LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
				{
					FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
			}
			DamageEffect(p->pBd, type);

		}
		else if (pEntity->mEntityType == ENTITY_TYPE::EXPLODER)
		{
			Exploder* p = static_cast<Exploder*>(pEntity);
			if (play->mOnePunchMan)
			{
				p->health = 0;
			}
			p->health -= play->mEWeapon->getDamage();
			srand(time(NULL));
			int iter = rand() % 100;
			if (iter < play->mEWeapon->getCritChance())
			{
				p->health -= play->mEWeapon->getDamage();
				std::cout << "\n Crit";
			}
			srand(iter + (iter / 6));
			iter = rand() % 100;
			if (iter < play->mEWeapon->mWeaponPerk->getProcChance())
			{
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WBURN_EFFECT)
				{
					BurningW* temp = static_cast<BurningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new BurningW(temp->getBurnDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WLIGHTNING_EFFECT)
				{
					LightningW* temp = static_cast<LightningW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getLightningDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
				if (play->mEWeapon->mWeaponPerk->getEffectType() == WEFFECT_TYPE::WFREEZING_EFFECT)
				{
					FreezingW* temp = static_cast<FreezingW*>(play->mEWeapon->mWeaponPerk);
					p->mAffectedBy = new LightningW(temp->getFreezingDamage(), temp->getProcChance(), temp->getEffectDuration());
				}
			}
			DamageEffect(p->pBd, type);
			}
		}
	}
}

void HitEvent::PerformPlayerDamage(Entity* pEntity)
{
	if (pEntity)
	{
		if (pEntity->mEntityType == ENTITY_TYPE::PLAYER)
		{
			Player* p = static_cast<Player*>(pEntity);
			srand((unsigned int)time(NULL));
			int iter = rand() % 100;
			if (!p->mGodMode)
			{
				if (iter < p->mEBody->getDodgeChance())
				{
					std::cout << "\n Dodge";
				}
				else
				{
					p->health -= 3;
					if (explosion)
					{
						p->health -= 6;
					}
				}
			}
					
		}
	}
}

void HitEvent::DamageEffect(Body * pObject, int t)
{
	{
		Particle temp;
		temp.age = 0.7;
		temp.position = Vector4(pObject->mPos_x, pObject->mPos_y, pObject->mPos_z, 0.5);
		temp.TextureIndex = t;
		temp.velocity = Vector4(3, 3, 3, 0.5);
		ParticleManager::GetInstance()->SphereEmit(40, temp);
	}
}
