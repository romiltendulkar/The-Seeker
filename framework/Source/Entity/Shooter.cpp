#include "../pch.h"
#include "Shooter.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ParticleManager.h"
#include "Player.h"
#include "..\Events\HitEvent.h"
#include "..\Events\EffectDamage.h"
#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\EventManager.h"

Shooter::Shooter() : Entity(ENTITY_TYPE::SHOOTER)
{
	mAffectedBy = nullptr;
}

Shooter::~Shooter() {}

void Shooter::Serialize(Json::Value root, unsigned int position)
{

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}
	dataRoot = root["Sleep"];
	if (!dataRoot.isNull())
	{
		sleep=dataRoot.asBool();
	}
	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		//pAs->obj->SetDiffuse(0.2f, 0.2f, 0.7f);
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Shooter";
	key = position;
	this->complete = true;
	this->runtime = false;
	ObjectFactoryManager::GetInstance()->LoadObject("Ebullet", key + 2);
	ObjectFactoryManager::GetInstance()->LoadObject("Ebullet", key + 3);
	ObjectFactoryManager::GetInstance()->LoadObject("Visible_Box", key + 1);
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_x = pBd->mPos_x;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_y = pBd->mPos_y;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pBd->mPos_z = pBd->mPos_z - 100;
	CreateUI();
	if (sleep)
	{
		health = 500;
	}
	else
	{
		health = 500;
	}
	ohealth = health;
}
void Shooter::Update(float deltaTime)
{
	Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 4].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 4].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 4].pTr->mPositionZ = pTr->mPositionZ + 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 4].pTr->mScaleX = 2 * (health / 500);
	float  distance = ((pBd->mPos_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x) * (pBd->mPos_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x) +
		(pBd->mPos_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y) * (pBd->mPos_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y));
	if (health > 0)
	{
		if(sleep)
		{
			Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
			{
				Particle temp;
				temp.age = 2.0;
				temp.position = Vector4(pTr->mPositionX, pTr->mPositionY, pTr->mPositionZ, 0.3);
				temp.TextureIndex = 2;
				temp.velocity = Vector4(0.7, 0.7, 0.7, 0.3);
				ParticleManager::GetInstance()->TimerEmit(60, temp);
			}
			return;
		}
		if (distance < 200 && (frame_counter == 100 || frame_counter == 200))
		{
			int loc;
			//std::cout << distance << std::endl;
			if (frame_counter == 100)
			{
				loc = key + 2;
			}
			else
			{
				loc = key + 3;
			}
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_x = pBd->mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_y = pBd->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mPos_z = pBd->mPos_z;
			player_x = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
			player_y = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
			result_x = player_x - pBd->mPos_x;
			result_y = player_y - pBd->mPos_y;
			float newRot = atan2(result_y, result_x);
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_x = 5.0f * cos(newRot);
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_y = 5.0f * sin(newRot);
			(*MemoryManager::GetInstance()->ObjectPool)[loc].pBd->mVelo_z = 5;
		}
		Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
		if (distance < 200)
		{
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
					pTr->mRotationZ += 5;
					if (pTr->mRotationZ > 360)
					{
						pTr->mRotationZ -= 360;
					}
				}
				else
				{
					pTr->mRotationZ -= 5;
					if (pTr->mRotationZ < 0)
					{
						pTr->mRotationZ += 360;
					}
				}
			}
			else if (pTr->mRotationZ > newRot1)
			{
				pTr->mRotationZ -= 5;
			}
			else
			{
				pTr->mRotationZ += 5;
			}
		}
		if (distance < 100)
		{
			player_x = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
			player_y = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
			result_x = player_x - pBd->mPos_x;
			result_y = player_y - pBd->mPos_y;
			float newRot = atan2(result_y, result_x);
			pBd->mVelo_x = -1.5 * cos(newRot);
			pBd->mVelo_y = -1.5 * sin(newRot);
			
			
		}
		else
		{
			pBd->mVelo_x = 0;
			pBd->mVelo_y = 0;
		}
		frame_counter++;
		if (frame_counter == 201)
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
	else
	{
		if (!point)
		{
			Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
			//pl->score++;
		}
		point = true;
		pBd->mPos_z = -500;
	}
}

void Shooter::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
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

			if (b1 == ENTITY_TYPE::SHOOTER)
			{
				if (b2 == ENTITY_TYPE::SHOOTER)
				{
					top = abs((pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight));
					bottom = abs((pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight));
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));
					if (top < bottom && top < left && top < right && top < in && top < out)//condition for top
					{
						pEvent->mpBody1->body_State = ON_SURFACE;
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight + pEvent->mpBody1->mHeight;
						pEvent->mpBody1->mVelo_z = 0;
					}
					else if (bottom < top && bottom < left && bottom < right && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody2->mVelo_z = 0;
						pEvent->mpBody2->body_State = ON_SURFACE;
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight + pEvent->mpBody2->mHeight;
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth - pEvent->mpBody2->mDepth;
						float t = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = pEvent->mpBody1->mVelo_y;
						pEvent->mpBody1->mVelo_y = t;
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth - pEvent->mpBody1->mDepth;
						float t = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = pEvent->mpBody1->mVelo_y;
						pEvent->mpBody1->mVelo_y = t;
					}
					else if (right < in && right < left && right < top && right < bottom && right < out)
					{
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth - pEvent->mpBody1->mWidth;
						float t = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = pEvent->mpBody1->mVelo_x;
						pEvent->mpBody1->mVelo_x = t;
					}
					else//condition for left
					{
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth - pEvent->mpBody2->mWidth;
						float t = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = pEvent->mpBody1->mVelo_x;
						pEvent->mpBody1->mVelo_x = t;
					}
				}
			}

			if (b1 == ENTITY_TYPE::SHOOTER)
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
			else if (b2 == ENTITY_TYPE::SHOOTER)
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

void Shooter::CreateUI()
{
	int curKey = key + 3;
	ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", key+4);
}

