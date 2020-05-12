#include "../pch.h"
#include "PuzzleTile.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include"../../Manager/PuzzleManager.h"
#include"../../Manager/ObjectFactoryManager.h"
#include "Player.h"
#include "Boss1.h"

#include "../Events/CollisionEvent.h"

PuzzleTile::PuzzleTile() : Entity(ENTITY_TYPE::PUZZLETILE)
{
}

PuzzleTile::~PuzzleTile() {}

void PuzzleTile::Serialize(Json::Value root, unsigned int position)
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
		//pAs->obj->SetDiffuse(0.2f, 0.2f, 0.7f);
		this->pAs = pAs;
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
		
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "PuzzleTile";
	key = position;
	this->complete = true;
	this->runtime = false;
}
void PuzzleTile::Update(float deltaTime)
{

	if (pBd)
		pBd->Update(deltaTime);
	Body* b1 = this->pBd;
	Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;//player body
	if (!solved)
	{
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
		{
			int c1 = PuzzleManager::GetInstance()->correct;
			if (c1 >= 2)
			{
				for (int i = 0; i < 9; i++)
				{
					int pos = ObjectFactoryManager::GetInstance()->LPTPositions[i];
					(*MemoryManager::GetInstance()->ObjectPool)[pos].pAs->obj->SetDiffuse(1, 1, 1);
				}
				solved = true;
				
			}
			if (solved)
			{
				Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
				pl->score=2;

			}
			else
			{
				if (PuzzleManager::GetInstance()->colorkey[c1] == color)//puzzle tile color key list == tile color
				{
					SetCorrectColor();
					PuzzleManager::GetInstance()->correct++;
					complete = false;
				}
				else
				{
					PuzzleManager::GetInstance()->Initialize();
					PuzzleManager::GetInstance()->correct = 0;
				}
			}
		}
	}

}

void PuzzleTile::SetColor(int color)
{
	if (color == 0)
		pAs->obj->SetDiffuse(1, 0, 0);
	if (color == 1)
		pAs->obj->SetDiffuse(0, 1, 0);
	if (color == 2)
		pAs->obj->SetDiffuse(0, 0, 1);
}

void PuzzleTile::SetCorrectColor()
{
	pAs->obj->SetDiffuse(1, 1, 1);
}

void PuzzleTile::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;
			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();
			if (b2 == ENTITY_TYPE::PUZZLETILE)
			{
				if (b1 == ENTITY_TYPE::EXPLODER || b1 == ENTITY_TYPE::SHOOTER || b1 == ENTITY_TYPE::BOSS1 || b1 == ENTITY_TYPE::CHARGER ||
					b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::BULLET || b1 == ENTITY_TYPE::COLLIDER ||
					b1 == ENTITY_TYPE::BLOCK || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::VIEWBOX || b1 == ENTITY_TYPE::WENEMY ||
					b1 == ENTITY_TYPE::SWITCH || b1 == ENTITY_TYPE::PILLAR)
				{
					top = abs((pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight));
					bottom = abs((pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight));
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						pEvent->mpBody1->body_State = ON_SURFACE;
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight + pEvent->mpBody1->mHeight;
						if (b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::WENEMY || b1 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody1->mVelo_x *= pEvent->mpBody2->mFrictionOnSurface;
							pEvent->mpBody1->mVelo_y *= pEvent->mpBody2->mFrictionOnSurface;
						}
						if (b1 == ENTITY_TYPE::BULLET)
						{
							pEvent->mpBody1->mVelo_z = (pEvent->mpBody2->mVelo_z * 0.50f);
						}
						if (b1 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody1->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight - pEvent->mpBody1->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody1->mCollidingSideRight = true;
						//c->mpBodies[0]->mRightMovement = false;
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth - pEvent->mpBody1->mWidth - 0.0001f;
						pEvent->mpBody1->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody1->mCollidingSideIn = true;
						//c->mpBodies[0]->mDownMovement = false;
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth + pEvent->mpBody1->mDepth + 0.0001f;
						pEvent->mpBody1->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_y = 0;
							//	c->mpBodies[0]->mAcc_y = -10;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody1->mCollidingSideOut = true;
						//c->mpBodies[0]->mUpMovement = false;
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth - pEvent->mpBody1->mDepth - 0.0001f;
						pEvent->mpBody1->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_y = 0;
							//	c->mpBodies[0]->mAcc_y = -60;
						}

					}
					else//condition for left
					{
						pEvent->mpBody1->mCollidingSideLeft = true;
						//c->mpBodies[0]->mLeftMovement = false;
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth + pEvent->mpBody1->mWidth + 0.0001f;
						pEvent->mpBody1->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_x = 0;
						}
					}
				}
			}
			else if (b1 == ENTITY_TYPE::PUZZLETILE)
			{
				if (b2 == ENTITY_TYPE::EXPLODER || b2 == ENTITY_TYPE::SHOOTER || b2 == ENTITY_TYPE::BOSS1 || b2 == ENTITY_TYPE::CHARGER ||
					b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::BULLET || b2 == ENTITY_TYPE::COLLIDER ||
					b2 == ENTITY_TYPE::BLOCK || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::VIEWBOX || b2 == ENTITY_TYPE::WENEMY ||
					b2 == ENTITY_TYPE::SWITCH || b2 == ENTITY_TYPE::PILLAR)
				{
					top = abs((pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight));
					bottom = abs((pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight) - (pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight));
					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						pEvent->mpBody2->body_State = ON_SURFACE;
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight + pEvent->mpBody2->mHeight;
						if (b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::WENEMY || b2 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody2->mVelo_x *= pEvent->mpBody1->mFrictionOnSurface;
							pEvent->mpBody2->mVelo_y *= pEvent->mpBody1->mFrictionOnSurface;
						}
						if (b2 == ENTITY_TYPE::BULLET)
						{
							pEvent->mpBody2->mVelo_z = abs(pEvent->mpBody2->mVelo_z * 0.50f);
						}
						if (b2 == ENTITY_TYPE::BLOCK)
						{
							pEvent->mpBody2->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight - pEvent->mpBody2->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody2->mCollidingSideRight = true;
						//c->mpBodies[1]->mRightMovement = false;
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth - pEvent->mpBody2->mWidth - 0.0001f;
						pEvent->mpBody2->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody2->mCollidingSideIn = true;
						//c->mpBodies[1]->mDownMovement = false;
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth + pEvent->mpBody2->mDepth + 0.0001f;
						pEvent->mpBody2->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							//	c->mpBodies[1]->mAcc_y = -10;
							pEvent->mpBody2->mVelo_y = 0;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody2->mCollidingSideOut = true;
						//c->mpBodies[1]->mUpMovement = false;
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth - pEvent->mpBody2->mDepth - 0.0001f;
						pEvent->mpBody2->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_y = 0;

						}
					}
					else//condition for left
					{
						pEvent->mpBody2->mCollidingSideLeft = true;
						//c->mpBodies[1]->mLeftMovement = false;
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth + pEvent->mpBody2->mWidth + 0.0001f;
						pEvent->mpBody2->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_x = 0;
						}
					}
				}
			}
		}
	}
}