#include "../pch.h"
#include "MovingPlatform.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "Player.h"

#include "..\Events\CollisionEvent.h"
#include "..\..\Manager\PhysicsManager.h"
#include "..\..\Manager\ObjectFactoryManager.h"

MovingPlatform::MovingPlatform() : Entity(ENTITY_TYPE::MOVINGPLATFORM)
{
	speed = 0.05;
	type = 1;
}

MovingPlatform::~MovingPlatform() {}

void MovingPlatform::Serialize(Json::Value root, unsigned int position)
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "MovingPlatform";
	key = position;
	this->complete = true;
	this->runtime = false;
}
void MovingPlatform::Update(float deltaTime)
{
	if (type == 1)
	{
		pBd->mPos_x += speed;
		pTr->mPositionX += speed;
	}
	else if (type == 2)
	{
		pBd->mPos_y += speed;
		pTr->mPositionY += speed;
	}
	//std::cout << "Type:" << type << std::endl;
}


void MovingPlatform::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::MOVINGPLATFORM)
			{
				if (b2 == ENTITY_TYPE::STOPPER)
				{
					MovingPlatform* mp = static_cast<MovingPlatform*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					mp->speed *= -1;
					PhysicsManager::GetInstance()->counter = 120;
					if (mp->type == 1)
					{
						pEvent->mpBody1->mPos_x += 3 * mp->speed;
						mp->pTr->mPositionX += 3 * mp->speed;
					}
					else
					{
						pEvent->mpBody1->mPos_y += 3 * mp->speed;
						mp->pTr->mPositionY += 3 * mp->speed;
					}
				}
				if (b2 == ENTITY_TYPE::MOVINGPLATFORM && PhysicsManager::GetInstance()->counter==0)
				{
					MovingPlatform* mp = static_cast<MovingPlatform*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody1->key].pEn);
					mp->speed *= -1;
					MovingPlatform* mp1 = static_cast<MovingPlatform*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					mp1->speed *= -1;
					PhysicsManager::GetInstance()->counter = 120;
					if (mp->type == 1)
					{
						pEvent->mpBody1->mPos_x += 6 * mp->speed;
						mp->pTr->mPositionX += 6 * mp->speed;
						pEvent->mpBody2->mPos_x += 6 * mp->speed;
						mp1->pTr->mPositionX += 6 * mp->speed;
					}
					else
					{
						pEvent->mpBody1->mPos_y += 6 * mp->speed;
						mp->pTr->mPositionY += 6 * mp->speed;
						pEvent->mpBody2->mPos_y += 6 * mp->speed;
						mp1->pTr->mPositionY += 6 * mp->speed;
					}
				}
				else if (b2 == ENTITY_TYPE::PLAYER)
				{
					/*float top, bottom, left, right, in, out;

					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));*/
					Player* p = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
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
					//if (right < left && right < in && right < out)//condition for right
					//{
					//	pEvent->mpBody2->mVelo_x = 10;
					//	//c->mpBodies[0]->mVelo_x = -20;
					//}
					//else if (in < right && in < left && in < out)
					//{
					//	pEvent->mpBody2->mVelo_y = -10;
					//	//c->mpBodies[0]->mVelo_y = 20;
					//}
					//else if (out < right && out < left && out < in)
					//{
					//	pEvent->mpBody2->mVelo_y = 10;
					//	//c->mpBodies[0]->mVelo_y = -20;
					//}
					//else//condition for left
					//{
					//	pEvent->mpBody2->mVelo_x = -10;
					//	//c->mpBodies[0]->mVelo_x = 20;
					//}
				}

			}
			else if (b2 == ENTITY_TYPE::MOVINGPLATFORM)
			{
				if (b1 == ENTITY_TYPE::STOPPER)
				{
					MovingPlatform* mp = static_cast<MovingPlatform*>((*MemoryManager::GetInstance()->ObjectPool)[pEvent->mpBody2->key].pEn);
					mp->speed *= -1;
					PhysicsManager::GetInstance()->counter = 120;
					if (mp->type == 1)
					{
						pEvent->mpBody2->mPos_x += 3 * mp->speed;
						mp->pTr->mPositionX += 3 * mp->speed;
					}
					else
					{
						pEvent->mpBody2->mPos_y += 3 * mp->speed;
						mp->pTr->mPositionY += 3 * mp->speed;
					}
				}
				else if (b1 == ENTITY_TYPE::PLAYER)
				{/*
					float top, bottom, left, right, in, out;

					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));*/

					Player* p = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
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
					//if (right < left && right < in && right < out)//condition for right
					//{
					//	pEvent->mpBody1->mVelo_x = 10;
					//	//c->mpBodies[1]->mVelo_x = -20;
					//}
					//else if (in < right && in < left && in < out)
					//{
					//	pEvent->mpBody1->mVelo_y = -10;
					//	//c->mpBodies[1]->mVelo_y = 20;
					//}
					//else if (out < right && out < left && out < in)
					//{
					//	pEvent->mpBody1->mVelo_y = 10;
					//	//c->mpBodies[1]->mVelo_y = -20;
					//}
					//else//condition for left
					//{
					//	pEvent->mpBody1->mVelo_x = -10;
					//	//c->mpBodies[1]->mVelo_x = 20;
					//}
				}
			}
		}
	}
}