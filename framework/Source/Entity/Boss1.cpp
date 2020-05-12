#include "../pch.h"
#include "Boss1.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/UIManager.h"
#include "Player.h"
#include "..\Events\HitEvent.h"
#include "..\Events\EffectDamage.h"

#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\EventManager.h"

Boss1::Boss1() : Entity(ENTITY_TYPE::BOSS1)
{
	mAffectedBy = nullptr;

}

Boss1::~Boss1() {}

void Boss1::Serialize(Json::Value root, unsigned int position)
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Boss1";
	key = position;
	this->complete = true;
	int i = 1;
	for (i = 1; i <= 72; i++)
	{
		ObjectFactoryManager::GetInstance()->LoadObject("Ebullet", key + i);
	}
	 pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
	 ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", key + i);
}
void Boss1::Update(float deltaTime)
{
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionX = pBd->mPos_x;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionY = pBd->mPos_y;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mPositionZ = pBd->mPos_z + 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mScaleX = 0;
	if (health > 0 )
	{
		if (pl->score>=2)
		{

			Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
			float dx, dy;
			dx = pBd->mPos_x - b2->mPos_x;
			dy = pBd->mPos_y - b2->mPos_y;
			float newRot1 = XMConvertToDegrees(atan2(dy, dx));
			if ((abs(pTr->mRotationZ - newRot1) < 15) || (abs(pTr->mRotationZ - newRot1) > 345))
			{
				pTr->mRotationZ = newRot1;
			}
			else if ((abs(pTr->mRotationZ - newRot1) > 180))
			{
				if (pTr->mRotationZ > newRot1)
				{
					pTr->mRotationZ += 15;
					if (pTr->mRotationZ > 360)
					{
						pTr->mRotationZ -= 360;
					}
				}
				else
				{
					pTr->mRotationZ -= 15;
					if (pTr->mRotationZ < 0)
					{
						pTr->mRotationZ += 360;
					}
				}
			}
			else if (pTr->mRotationZ > newRot1)
			{
				pTr->mRotationZ -= 15;
			}
			else
			{
				pTr->mRotationZ += 15;
			}

			(*MemoryManager::GetInstance()->ObjectPool)[key + 73].pTr->mScaleX = 2 * (health / 100);
			UIManager::GetInstance()->SetGoal("Beat the Boss!");
			float angle = 0.17632698;

			frame_counter++;
			//std::cout << round_counter << std::endl;
			if (round_counter == 5 && mode == 1)
			{
				mode = 2;
				frame_counter = 0;
				round_counter = 0;
			}
			if (frame_counter == 100 && mode == 1)
			{
				for (int i = 1; i <= 36; i++)
				{
					int loc = key + i;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 5.0f * cos(angle);
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 5.0f * sin(angle);
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
					angle = angle + 0.17632698;
				}
			}
			else if (frame_counter == 200 && mode == 1)
			{
				for (int i = 33; i <= 72; i++)
				{
					int loc = key + i;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 5.0f * cos(angle);
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 5.0f * sin(angle);
					(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
					angle = angle + 0.17632698;
				}
				round_counter++;
			}
			if (frame_counter % 10 == 0 && mode == 2)
			{
				int loc = key + pos;
				++pos;
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 5.0f * cos(angle_proxy);
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 5.0f * sin(angle_proxy);
				(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mAcc_z = 0;
				angle_proxy = angle_proxy + 0.17632698;
			}
			if (pos == 73)
			{
				pos = 1;
				angle_proxy = 0.17632698;
			}
			if (frame_counter == 200 && mode == 1)
			{
				frame_counter = 0;
			}
			if (mode == 2 && frame_counter == 1000)
			{
				frame_counter = 0;
				mode = 1;
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
		MemoryManager::GetInstance()->mpOwner->ChangeLevel("Level4", true);
	}
	

	
}


void Boss1::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pl->score >= 2)
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

				if (b1 == ENTITY_TYPE::BOSS1)
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
				else if (b2 == ENTITY_TYPE::BOSS1)
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