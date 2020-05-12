#include "../pch.h"
#include "Block.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "../Events/CollisionEvent.h"

Block::Block() : Entity(ENTITY_TYPE::BLOCK)
{

}

Block::~Block() {}

void Block::Serialize(Json::Value root, unsigned int position)
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
		pAs->obj->SetDiffuse(255, 100, 0);
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
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Block";
	this->complete = true;
	this->runtime = false;
	key = position;
}

void Block::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);

}

void Block::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;

			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::BLOCK)
			{
				if (b2 == ENTITY_TYPE::PLAYER)
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
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody2->mVelo_x *= pEvent->mpBody1->mFrictionOnSurface;
							pEvent->mpBody2->mVelo_y *= pEvent->mpBody1->mFrictionOnSurface;
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
							pEvent->mpBody1->mVelo_x = -2 * pEvent->mpBody2->mVelo_x;
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
							pEvent->mpBody1->mVelo_y = -2 * pEvent->mpBody2->mVelo_y;
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
							pEvent->mpBody1->mVelo_y = -2 * pEvent->mpBody2->mVelo_y;
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
							pEvent->mpBody1->mVelo_x = -2 * pEvent->mpBody2->mVelo_x;
							pEvent->mpBody2->mVelo_x = 0;
						}
					}
				}
				if (b2 == ENTITY_TYPE::BLOCK)
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
			else if (b2 == ENTITY_TYPE::BLOCK)
			{
				if (b1 == ENTITY_TYPE::PLAYER)
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
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							pEvent->mpBody1->mVelo_x *= pEvent->mpBody2->mFrictionOnSurface;
							pEvent->mpBody1->mVelo_y *= pEvent->mpBody2->mFrictionOnSurface;
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
							pEvent->mpBody2->mVelo_x = -2 * pEvent->mpBody1->mVelo_x;
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
							pEvent->mpBody2->mVelo_y = -2 * pEvent->mpBody1->mVelo_y;
							pEvent->mpBody1->mVelo_y = 0;
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
							pEvent->mpBody2->mVelo_y = -2 * pEvent->mpBody1->mVelo_y;
							pEvent->mpBody1->mVelo_y = 0;
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
							pEvent->mpBody2->mVelo_x = -2 * pEvent->mpBody1->mVelo_x;
							pEvent->mpBody1->mVelo_x = 0;
						}
					}
				}
			}
		}
	}
}
