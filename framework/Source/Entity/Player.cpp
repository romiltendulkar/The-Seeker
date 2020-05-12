#include "../pch.h"
#include "Player.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Controller.h"
#include "../Components/Body.h"
#include "../Components/Animation.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../Entity/HealthBar.h"
#include "../Entity/PointLight.h"
#include "..//..//Manager/CollisionManager.h"
#include "../Entity/TimeBar.h"
#include "../Events/HitEvent.h"
#include "../Events/CollisionEvent.h"
#include "../../Manager/UIManager.h"



Player::Player() : Entity(ENTITY_TYPE::PLAYER)
{
	health = 100.0f;
	if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
	{
		
	}
	else
	{
		time = 0.0f;
	}
	life = 3;
	bosskill = 0;
	mLoseCounter = 0;
	time = 10000.0f;
	mDashCounter = 0;
	mDashCounter = 0;
	mWinCounter = 0;
	mInventory = std::vector<Gear*>();
	mEWeapon = new Weapon();
	mEBody = new Armor();
	mEHelmet = new Armor(5, 1.0f, "Leather Helmet", ARMOR_TYPE::TYPE_HELMET, GEAR_RARITY::COMMONGEAR, (unsigned int)1);
}



Player::~Player() {
	delete mEWeapon;
	delete mEBody;
	delete mEHelmet;


}

void Player::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];


	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		//pAs->obj->SetDiffuse(1, 1, 0);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}

	//Controller
	dataRoot = root["Controller"];

	if (!dataRoot.isNull())
	{
		Controller* pCr = new Controller();
		pCr->Serialize(dataRoot, position);
		this->pCr = pCr;
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Player";
	key = position;
	bosskill = 0;
	mLoseCounter = 0;

	this->complete = true;
	this->runtime = false;
	//CreateUI();
	//CreatePointLight();
	ObjectFactoryManager::GetInstance()->LoadObject("LP", key + 1);
	ObjectFactoryManager::GetInstance()->LoadObject("RP", key + 2);
}

void Player::Update(float deltaTime)
{
	/*std::cout << (*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mpEnowner->mEntityName << std::endl;*/
	if (health >= 100)
	{
		health = 100;
	}
	mWeapon = 0.85f * (Equipped + 1);
	mRangedWeapon = 0.85f * (REquipped + 1);
	if (mDashCounter > 0)
	{
		mDashCounter--;
		if (mDashCounter == 0)
		{
			mDashCounterCD = 30;
		}
		if (pBd->mFacingUp == true && pBd->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 3*(0.2f / 1.414f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 3*(0.2f / 1.414f);
		}
		else if (pBd->mFacingUp == true && pBd->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 3*0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 3*0.2f / sqrtf(2.0f);
		}
		else  if (pBd->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 3*0.2f;
		}
		if (pBd->mFacingDown == true && pBd->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 3*0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 3*0.2f / sqrtf(2.0f);
		}
		else if (pBd->mFacingDown == true && pBd->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 3*0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 3*0.2f / sqrtf(2.0f);
		}
		else if (pBd->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 3*0.2f;
		}
		if (pBd->mFacingRight == true && pBd->mFacingDown == false && pBd->mFacingUp == false)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 3*0.2f;
		}
		else if (pBd->mFacingLeft == true && pBd->mFacingDown == false && pBd->mFacingUp == false)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 3*0.2f;
		}
	}
	if (mDashCounterCD > 0)
	{
		mDashCounterCD--;
	}
	if (pBd->mFacingDown == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x - 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y - 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = pBd->mPos_z;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_x = pBd->mPos_x + 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_y = pBd->mPos_y - 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_z = pBd->mPos_z;
	}
	else if (pBd->mFacingLeft == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x - 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y + 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = pBd->mPos_z;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_x = pBd->mPos_x - 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_y = pBd->mPos_y - 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_z = pBd->mPos_z;
	}
	else if (pBd->mFacingRight == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x + 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y - 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = pBd->mPos_z;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_x = pBd->mPos_x + 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_y = pBd->mPos_y + 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_z = pBd->mPos_z;
	}
	else if (pBd->mFacingUp == true)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x + 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y + 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = pBd->mPos_z;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_x = pBd->mPos_x - 0.5;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_y = pBd->mPos_y + 1;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_z = pBd->mPos_z;
	}
	//check if we can punch an enemy:
	for (int k = 0; k < ObjectFactoryManager::GetInstance()->LEnemyPositions.size(); k++)
	{
		Body* b1 = (*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->LEnemyPositions.at(k)].pBd;
		Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd;
		Body* b3 = (*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd;

		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))//Right
		{
			//std::cout << ObjectFactoryManager::GetInstance()->Pathfinder_pos << std::endl;
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->UseDiffuseColor(1);
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->SetDiffuse(1, 0, 0);
			break;
		}
		else if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b3->mShape, b3->mPos_x, b3->mPos_y, b3->mPos_z))//Left
		{
			//std::cout << ObjectFactoryManager::GetInstance()->Pathfinder_pos << std::endl;
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->UseDiffuseColor(1);
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->SetDiffuse(1, 0, 0);
			break;
		}
		else
		{
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->SetDiffuse(1, 1, 0);
			(*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->Pathfinder_pos].pAs->obj->UseDiffuseColor(0);
		}
	}
	if(!(mLoseCounter>0|| mWinCounter>0))
		pCr->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
	if (!pCr)
	{
		//std::cout << "not pcr" << std::endl;
	}
	if (pBd)
		pBd->Update(deltaTime);
	UIManager::GetInstance()->SetGoal("\n\n\n2    Life x" + to_string(life));
	if (health < 0)
	{
		health = 0;
		life--;

		if (life >-1)
		{
			UIManager::GetInstance()->SetGoal("\n\n\n2    Life x" + to_string(life));

			MemoryManager::GetInstance()->mpOwner->ChangeLevel(MemoryManager::GetInstance()->mpOwner->mCurrentLevel, true);
		}
		else 
		{
			UIManager::GetInstance()->SetGoal("\n\n\n2    Life x-1");
			if (!lose)
			{
				lose = ResourceManager::GetInstance()->LoadTexture("GameOver.png");
			}
			UIManager::GetInstance()->LoadHalfScreen(lose, 1, 0);
		}
		bosskill = 0;
	}
	if (health <= 0&& life<-1)
	{
		mLoseCounter++;
		pCr->usable=false;
		if (mLoseCounter > 300)
		{
			UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
			UIManager::GetInstance()->IOShowStat(0);
			UIManager::GetInstance()->IOShowGoal(0);

			MemoryManager::GetInstance()->mpOwner->ChangeLevel("MenuLevel", 0);
			pCr->usable = true;
			mWinCounter = 0;
			mLoseCounter = 0;
			return;
		}
	}
	
	if (bosskill >= MemoryManager::GetInstance()->bosscout&& (MemoryManager::GetInstance()->bosscout>0))
	{
		pCr->usable = false;
		mWinCounter++;
		if (!win)
		{
			win= ResourceManager::GetInstance()->LoadTexture("GameCleared.png");
		}
		UIManager::GetInstance()->LoadHalfScreen(win,1, 0);

		UIManager::GetInstance()->SetDialogue("You just save the world! Thank you!");

		if (mWinCounter > 360)
		{
			UIManager::GetInstance()->LoadHalfScreen(nullptr, 0);
			UIManager::GetInstance()->IOShowStat(0);
			MemoryManager::GetInstance()->mpOwner->ChangeLevel("CreditsLevel", 0);
			mWinCounter = 0;
		}
	}

}

void Player::CreateUI()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RPG || MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
	{
		eh = true;
		ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", curKey);
		//
		////if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
		//{
		//	ObjectFactoryManager::GetInstance()->LoadObject("TimeBar", curKey + 1);
		//}
	}
	//else
	//{
	//	//ObjectFactoryManager::GetInstance()->LoadObject("Cat", curKey);
	//}
	//
}

void Player::CreatePointLight()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	ObjectFactoryManager::GetInstance()->LoadObject("PointLight", curKey);
	lpos = curKey;
}

void Player::CopyStats(Player* pOrg)
{
	health = 100.0f;
	time = pOrg->time;
	mDashCounter = pOrg->mDashCounter;
	mGodMode = pOrg->mGodMode;
	mOnePunchMan = pOrg->mOnePunchMan;
	
	mEWeapon->CopyStats(pOrg->mEWeapon);
	mEBody->CopyStats(pOrg->mEBody);
	mEHelmet->CopyStats(pOrg->mEHelmet);
}

void Player::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::HIT_EVENT)
		{
			HitEvent* newHit = static_cast<HitEvent*>(pEventToHandle);
			newHit->PerformPlayerDamage(this);

		}
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::PLAYER)
			{
				if (b2 == ENTITY_TYPE::EBULLET)
				{
					(*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pCr->usable = false;
					//player->health -= 10;
					HitEvent* newHit = new HitEvent();
					newHit->PerformPlayerDamage(this);
					delete newHit;
				}
			}
			else if (b2 == ENTITY_TYPE::PLAYER)
			{
				if (b1 == ENTITY_TYPE::EBULLET)
				{
					(*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pCr->usable = false;
					//player->health -= 10;
					HitEvent* newHit = new HitEvent();
					newHit->PerformPlayerDamage(this);
					delete newHit;
				}
			}
		}
	}
}

void Player::ToggleGodMode()
{
	mGodMode = !mGodMode;
}

void Player::ToggleOneHitKill()
{
	mOnePunchMan = !mOnePunchMan;
}
