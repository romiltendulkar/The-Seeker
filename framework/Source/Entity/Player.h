#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
#include "..\Inventory\Armor.h"
#include "..\Inventory\Weapon.h"

class Controller;
class Animation;
class Body;
class Transform;
class Player : public Entity
{
public:
	enum Weapons
	{
		Wooden_Sword,
		Iron_Sword
	};
	enum Ranged_Weapons
	{
		Slingshot,
		Fireball
	};
	/**
   In this constructor we initialize the variables.
	*/
	Player();
	virtual ~Player();
	/**
	Serializes asset
	Root : JSON Root
	key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int position);
	/**
	   In this function, every component of the said entity is updated.

	  \param rdeltaTime | Delta time between frames.
	*/
	void Update(float deltaTime);
	/**
	   In this function, UI for the player, health bar or time bar is created.
	*/
	void CreateUI();
	/**
	   In this function, point light at the player's position is created
	*/
	void CreatePointLight();
	void HandleEvent(Event *pEvent);
	void CopyStats(Player* pOrg);
	void ToggleGodMode();
	void ToggleOneHitKill();
	bool eh = false;
	bool sound = false;
public:
	unsigned int key;//entity position in the pool
	unsigned int hPos;
	Controller* pCr;
	Animation* pAn;
	Body* pBd;
	Transform* pTr;
	int lpos;
	float health;
	int mDashCounter;
	int mDashCounterCD;
	int mWinCounter = 0;
	int mLoseCounter = 0;
	
	int life = 3;
	float time;
	int score = 0;
	DXTexture* win;
	DXTexture* lose;
public:

	float mDefense = 10;
	float mAttack = 10;

	float mArmor;
	float mWeapon;
	float mRangedWeapon;
	bool mGodMode = false;
	bool mOnePunchMan = false;
	bool spotcheck = false;
	Weapon* mEWeapon;
	Armor* mEHelmet;
	Armor* mEBody;
	bool pressed_left = false;

	Weapons Equipped = Wooden_Sword;
	Ranged_Weapons REquipped = Fireball;
	int bosskill = 0;
	std::vector<Gear* > mInventory;

};
