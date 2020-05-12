#include "../pch.h"
#include "Collider.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include"../../Manager/ObjectFactoryManager.h"
#include "../../Manager/EventManager.h"
#include "../Entity/Player.h"
#include "../Events/CollisionEvent.h"
#include "..\Events\HitEvent.h"

Collider::Collider() : Entity(ENTITY_TYPE::COLLIDER)
{
	pBd = nullptr;
	pAn = nullptr;
}

Collider::~Collider() {}

void Collider::Serialize(Json::Value root, unsigned int position)
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
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Collider";
	this->complete = true;
	this->runtime = false;
	key = position;
}

void Collider::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);


	if (pBd->mVelo_x > 0)
	{
		pTr->mRotationZ += pBd->mVelo_x/5;
	}
	if (pBd->mVelo_x < 0)
	{
		pTr->mRotationZ += pBd->mVelo_x / 5;
	}
}

void Collider::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;

			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::COLLIDER)
			{
				if (b2 == ENTITY_TYPE::PLAYER)
				{
					/*float top, bottom, left, right, in, out;

					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));*/
					Player* p = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, p->key);
					delete newHit;
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - pEvent->mpBody1->mPos_x;
					float result_y = player_y - pEvent->mpBody1->mPos_y;
					float newRot = atan2(result_y, result_x);
					p->pBd->mVelo_x = 50 * cos(newRot);
					p->pBd->mVelo_y = 50 * sin(newRot);
					for (int k = 0; k < ObjectFactoryManager::GetInstance()->LWallPositions.size(); k++)
					{
						Body* b1 = (*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->LEnemyPositions.at(k)].pBd;
						Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
						float px, py;
						px = b2->mPos_x + (p->pBd->mVelo_x * 0.5 * 0.016f);
						py = b2->mPos_y + (p->pBd->mVelo_y * 0.5 * 0.016f);
						if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, px, py, b2->mPos_z))
						{
							(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_x = 0;
							(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_y = 0;
						}
					}
				}
				if (b2 == ENTITY_TYPE::COLLIDER)
				{
					top = abs((pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight));
					bottom = abs((pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight));
					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth - pEvent->mpBody2->mWidth - 0.01f;
				
						float init_momentum = pEvent->mpBody2->mVelo_x * pEvent->mpBody2->mMass + pEvent->mpBody1->mVelo_x * pEvent->mpBody1->mMass;
						float diff_v1v2;
						diff_v1v2 = (pEvent->mpBody2->mVelo_x - pEvent->mpBody1->mVelo_x);
						float m2diff = pEvent->mpBody1->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = Im_m2dif / (pEvent->mpBody2->mMass + pEvent->mpBody1->mMass);
						pEvent->mpBody1->mVelo_x = temp + pEvent->mpBody2->mVelo_x - pEvent->mpBody1->mVelo_x;
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth + pEvent->mpBody2->mDepth + 0.01f;
						float init_momentum = pEvent->mpBody2->mVelo_y * pEvent->mpBody2->mMass + pEvent->mpBody1->mVelo_y * pEvent->mpBody1->mMass;
						float diff_v1v2;
						diff_v1v2 = (pEvent->mpBody2->mVelo_y - pEvent->mpBody1->mVelo_y);
						float m2diff = pEvent->mpBody1->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = Im_m2dif / (pEvent->mpBody2->mMass + pEvent->mpBody1->mMass);
						pEvent->mpBody1->mVelo_y = temp + pEvent->mpBody2->mVelo_y - pEvent->mpBody1->mVelo_y;
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth - pEvent->mpBody2->mDepth - 0.01f;
					
						float init_momentum = pEvent->mpBody2->mVelo_y * pEvent->mpBody2->mMass + pEvent->mpBody1->mVelo_y * pEvent->mpBody1->mMass;
						float diff_v1v2;
						diff_v1v2 = (pEvent->mpBody2->mVelo_y - pEvent->mpBody1->mVelo_y);
						float m2diff = pEvent->mpBody1->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = Im_m2dif / (pEvent->mpBody2->mMass + pEvent->mpBody1->mMass);
						pEvent->mpBody1->mVelo_y = temp + pEvent->mpBody2->mVelo_y - pEvent->mpBody1->mVelo_y;
					}
					else//condition for left
					{
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth + pEvent->mpBody2->mWidth + 0.01f;
						float init_momentum = pEvent->mpBody2->mVelo_x * pEvent->mpBody2->mMass + pEvent->mpBody1->mVelo_x * pEvent->mpBody1->mMass;
						float diff_v1v2;
						diff_v1v2 = (pEvent->mpBody2->mVelo_x - pEvent->mpBody1->mVelo_x);
						float m2diff = pEvent->mpBody1->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = Im_m2dif / (pEvent->mpBody2->mMass + pEvent->mpBody1->mMass);
						pEvent->mpBody1->mVelo_x = temp + pEvent->mpBody2->mVelo_x - pEvent->mpBody1->mVelo_x;
					}
				}
			}
			else if (b2 == ENTITY_TYPE::COLLIDER)
			{
				if (b1 == ENTITY_TYPE::PLAYER)
				{
					/*float top, bottom, left, right, in, out;

					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));*/
					Player* p = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, p->key);
					delete newHit;
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - pEvent->mpBody2->mPos_x;
					float result_y = player_y - pEvent->mpBody2->mPos_y;
					float newRot = atan2(result_y, result_x);
					p->pBd->mVelo_x = 50 * cos(newRot);
					p->pBd->mVelo_y = 50 * sin(newRot);
					for (int k = 0; k < ObjectFactoryManager::GetInstance()->LWallPositions.size(); k++)
					{
						Body* b1 = (*MemoryManager::GetInstance()->ObjectPool)[ObjectFactoryManager::GetInstance()->LWallPositions.at(k)].pBd;
						Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
						float px, py;

						px = b2->mPos_x + (p->pBd->mVelo_x * 0.5 * 0.016f);
						py = b2->mPos_y + (p->pBd->mVelo_y * 0.5 * 0.016f);
						if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, px, py, b2->mPos_z))
						{
							(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_x = 0;
							(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_y = 0;
						}
					}
				}
			}
		}
	}
}
