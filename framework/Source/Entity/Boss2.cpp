#include "../pch.h"
#include "Boss2.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/UIManager.h"
#include "Player.h"
#include "..\Events\HitEvent.h"
#include "..\Events\EffectDamage.h"

#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\EventManager.h"

Boss2::Boss2() : Entity(ENTITY_TYPE::BOSS2)
{
	mAffectedBy = nullptr;

}

Boss2::~Boss2() {}

void Boss2::Serialize(Json::Value root, unsigned int position)
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
		//pAs->obj->SetDiffuse(0.0f, 0.0f, 1.0f);
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Boss2";
	key = position;
	this->complete = true;
	int i = 1;
	for (i = 1; i <= 72; i++)
	{
		ObjectFactoryManager::GetInstance()->LoadObject("Ebullet", key + i);
	}
	pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
	ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", key + i);
	bossactive = true;
}
void Boss2::Update(float deltaTime)
{
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionX = pBd->mPos_x;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionY = pBd->mPos_y;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionZ = pBd->mPos_z + 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mScaleX = 0;
	if (health > 0)
	{
		{

			float player_x = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
			float player_y = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
			float result_x = player_x - pBd->mPos_x;
			float result_y = player_y - pBd->mPos_y;
			float newRot1 = atan2(result_y, result_x);
			newRot1 = pTr->mRotationZ += 1;
			
			(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mScaleX = 2 * (health / 100);
			float angle = 0.17632698/2.5f;
			//angle = (PI * newRot1) / 180;
			angle = XMConvertToRadians(newRot1);
			frame_counter++;
			//std::cout << round_counter << std::endl;
			if (health >= 150)
			{
				if (pBd->mVelo_z == 0 && round_counter == 0)
				{
					for (int i = 1; i <= 36; i++)
					{
						int loc = key + i;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 3.0f * cos(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 3.0f * sin(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_z = 10.0f;
						//(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
						angle = angle + 0.17632698;
					}
					angle = XMConvertToRadians(newRot1);
					for (int i = 33; i <= 72; i++)
					{
						int loc = key + i;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 3.0f * cos(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 3.0f * sin(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_z = 10.0f;
						//(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
						angle = angle + 0.17632698;
					}
					round_counter++;
				}
				else if (pBd->mVelo_z == 0 && round_counter == 1)
				{
					
					for (int i = 1; i <= 36; i++)
					{
						int loc = key + i;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 1.0f * cos(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 1.0f * sin(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_z = 10.0f;
						//(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
						angle = angle + 0.17632698;
					}
					angle = XMConvertToRadians(newRot1);
					for (int i = 33; i <= 72; i++)
					{
						int loc = key + i;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 1.0f * cos(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 1.0f * sin(angle);
						(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_z = 10.0f;
						//(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
						angle = angle + 0.17632698;
					}
					round_counter = 2;
				}
				if (frame_counter == 119)
				{
					round_counter = 0;
				}
				if (frame_counter == 120)
				{
					//std::cout << "hua" << std::endl;
					pBd->mVelo_z = 5;
					pBd->mPos_z += 0.1;
					pBd->body_State = FALLING;
				}
			}
			
			if (health < 150 && frame_counter == 120)
			{
				std::cout << "HAIL" << std::endl;
				frame_counter = 0;
				float player_x = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
				float player_y = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
				float result_x = player_x - pBd->mPos_x;
				float result_y = player_y - pBd->mPos_y;
				float newRot1 = atan2(result_y, result_x);
				pBd->mVelo_x =  5.0f * cos(newRot1);
				pBd->mVelo_y =  5.0f * sin(newRot1);
			}
			if (frame_counter >= 120)
			{
				frame_counter = 0;
			}
			if (mAffectedBy)
			{
				EffectDamage* pED = new EffectDamage();
				pED->applyEffect(mAffectedBy, this, deltaTime);
				delete pED;
				mAffectedBy->reduceTime(deltaTime);
				if (mAffectedBy->getEffectDuration() <= 0)
				{
					delete mAffectedBy;
					mAffectedBy = nullptr;
				}
			}


		}

	}
	else
	{
		pBd->mPos_z = -500;
		//MemoryManager::GetInstance()->mpOwner->ChangeLevel(MemoryManager::GetInstance()->mpOwner->mNextLevel, true);
		Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
		if (bossactive)
		{
			player->bosskill++;
			bossactive = !bossactive;
		}

	}



}


void Boss2::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		{
			if (pEventToHandle->mEventType == EVENT_TYPE::HIT_EVENT)
			{
				HitEvent* pHit = static_cast<HitEvent*>(pEventToHandle);

				pHit->PerformEnemyDamage(this);
				delete pEventToHandle;
			}
			else if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
			{
				float top, bottom, left, right, in, out;

				CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
				ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
				ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

				if (b1 == ENTITY_TYPE::BOSS2)
				{
					if (b2 == ENTITY_TYPE::PLAYER)
					{
						left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
						right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
						in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
						out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));

						(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

						HitEvent* newHit = new HitEvent();
						EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
						delete newHit;


						if (right < left && right < in && right < out)//condition for right
						{
							pEvent->mpBody2->mVelo_x = 10;
							//c->mpBodies[0]->mVelo_x = -20;
						}
						else if (in < right && in < left && in < out)
						{
							pEvent->mpBody2->mVelo_y = -10;
							//c->mpBodies[0]->mVelo_y = 20;
						}
						else if (out < right && out < left && out < in)
						{
							pEvent->mpBody2->mVelo_y = 10;
							//c->mpBodies[0]->mVelo_y = -20;
						}
						else//condition for left
						{
							pEvent->mpBody2->mVelo_x = -10;
							//c->mpBodies[0]->mVelo_x = 20;
						}

					}
				}
				else if (b2 == ENTITY_TYPE::BOSS2)
				{
					if (b1 == ENTITY_TYPE::PLAYER)
					{
						left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
						right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
						in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
						out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));


						(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

						HitEvent* newHit = new HitEvent();
						EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
						delete newHit;

						if (right < left && right < in && right < out)//condition for right
						{
							pEvent->mpBody1->mVelo_x = 10;
							//c->mpBodies[1]->mVelo_x = -20;
						}
						else if (in < right && in < left && in < out)
						{
							pEvent->mpBody1->mVelo_y = -10;
							//c->mpBodies[1]->mVelo_y = 20;
						}
						else if (out < right && out < left && out < in)
						{
							pEvent->mpBody1->mVelo_y = 10;
							//c->mpBodies[1]->mVelo_y = -20;
						}
						else//condition for left
						{
							pEvent->mpBody1->mVelo_x = -10;
							//c->mpBodies[1]->mVelo_x = 20;
						}
					}
				}
			}

		}

	}

}