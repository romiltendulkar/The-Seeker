/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Gear.h
Purpose: This is the base class for various inventory items
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 02/12/2019
- End Header --------------------------------------------------------*/

#include "..\pch.h"
#include "LootDrop.h"
#include "..\Inventory\Weapon.h"
#include "..\Inventory\Armor.h"
#include "Player.h"

LootDrop::LootDrop() : Entity(ENTITY_TYPE::LOOTDROP)
{
	Looted = false;
	drop = nullptr;
}

LootDrop::LootDrop(float posx, float posy, float scalex, float scaley) : Entity(ENTITY_TYPE::LOOTDROP)
{

}

LootDrop::~LootDrop()
{

}

void LootDrop::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		//pAs->obj->SetDiffuse(0.5f, 0.5f, 0.1f);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//dataRoot = root["LootWeapon"];
	//if (!dataRoot.isNull())
	
		float damage;
		float crit;
		unsigned int wlvl;
		std::string name;
		dataRoot = root["WeaponDamage"];
		if (!dataRoot.isNull())
		{
			damage = dataRoot.asFloat();
		}
		dataRoot = root["WeaponCrit"];
		if (!dataRoot.isNull())
		{
			crit = dataRoot.asFloat();
		}
		dataRoot = root["WeaponLevel"];
		if (!dataRoot.isNull())
		{
			wlvl = dataRoot.asInt();
		}
		dataRoot = root["WeaponName"];
		if (!dataRoot.isNull())
		{
			name = dataRoot.asString();
		}
		Weapon* pW = new Weapon(damage, crit, name, GEAR_RARITY::UNCOMMONGEAR, wlvl);
		drop = pW;

	
	dataRoot = root["LootArmor"];
	if (!dataRoot.isNull())
	{
		float def;
		float dodge;
		int lvl;
		std::string name;
		dataRoot = root["ArmorDefense"];
		def = dataRoot.asFloat();
		dataRoot = root["ArmorDodge"];
		dodge = dataRoot.asFloat();
		dataRoot = root["ArmorLevel"];
		lvl = dataRoot.asInt();
		dataRoot = root["ArmorName"];
		name = dataRoot.asString();
		Armor *pA = new Armor(def, dodge, name, ARMOR_TYPE::TYPE_BODY, GEAR_RARITY::UNCOMMONGEAR, lvl);
		drop = pA;
	}
	

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Platform";
	key = position;
	this->complete = true;
	this->runtime = false;
}

void LootDrop::Update(float deltaTime)
{
	if (!Looted)
	{
		Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(pBd->mShape, pBd->mPos_x, pBd->mPos_y, pBd->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
		{
			if (drop)
			{
				Player* pP = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
				if (drop->getType() == GEAR_TYPE::TYPE_WEAPON)
				{
					Weapon* pw = static_cast<Weapon*>(drop);
					pP->mEWeapon->upgradeDamage(pw->getDamage());
					pP->mEWeapon->upgradeCritChance(pw->getCritChance());
					pP->mEWeapon->changeName(pw->getWepName());
					pP->mEWeapon->upgradeGearLevel(pw->getGearLevel());
					delete drop;
					drop = nullptr;
				}
				else if(drop->getType() == GEAR_TYPE::TYPE_ARMOR)
				{
					Armor* pA = static_cast<Armor*>(drop);
					pP->mEBody->upgradeDefense(pA->getDefense());
					pP->mEBody->upgradeDodge(pA->getDodgeChance());
					pP->mEBody->changeName(pA->getArmorName());
					pP->mEBody->upgradeGearLevel(pA->getGearLevel());
					delete drop;
					drop = nullptr;
				}
				Looted = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX = 0;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY = 0;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleZ = 0;
			}	
		}
	}
}