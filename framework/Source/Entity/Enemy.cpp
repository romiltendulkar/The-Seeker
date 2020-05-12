#include "../pch.h"
#include "Enemy.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "..\..\Manager\CollisionManager.h"


bool SearchArea(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z)
{
	ShapeAABB* pAABB1 = static_cast<ShapeAABB*>(pShape1);
	float bottom1 = pos1y - pAABB1->mBottom;
	float top1 = pos1y + pAABB1->mTop;
	float in1 = pos1z + pAABB1->mIn;
	float out1 = pos1z - pAABB1->mOut;
	float left1 = pos1x - pAABB1->mLeft;
	float right1 = pos1x + pAABB1->mRight;

	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);
	float bottom2 = pos1y - pAABB2->pOwnerBody->mScale_y;
	float top2 = pos1y + pAABB2->pOwnerBody->mScale_y;
	float in2 = pos1z + pAABB2->pOwnerBody->mScale_z;
	float out2 = pos1z - pAABB2->pOwnerBody->mScale_z;
	float left2 = pos1x - pAABB2->pOwnerBody->mScale_x;
	float right2 = pos1x + pAABB2->pOwnerBody->mScale_x;

	if (bottom1 > top2 || top1 < bottom2)
	{
		return false;
	}
	if (left1 > right2 || right1 < left2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Enemy::Enemy() : Entity(ENTITY_TYPE::ENEMY)
{
	pBd = nullptr;
	key = -1;
	health = 100.0f;
	pSearchArea = new ShapeAABB();
	pSearchArea->mBottom = pSearchArea->mTop = 4;
	pSearchArea->mLeft = pSearchArea->mRight = 4;
	pSearchArea->mIn = pSearchArea->mOut = 4;
	pSearchBody = new Body();
	pSearchArea->SetBody(pSearchBody);
	pSearchBody->mScale_x = 4;
	pSearchBody->mScale_y = 4;
	pSearchBody->mScale_z = 4;
	pSearchBody->mAng_x = pSearchBody->mAng_y = pSearchBody->mAng_z = 0;
}

Enemy::~Enemy() {}

void Enemy::Serialize(Json::Value root, unsigned int position)
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
		//pAs->obj->SetDiffuse(1, 0, 0);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	pSearchBody->mPos_x = pBd->mPos_x;
	pSearchBody->mPos_y = pBd->mPos_y;
	pSearchBody->mPos_z = pBd->mPos_z;
	
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Enemy";
	key = position;
	this->complete = true;

	CreateUI();
	pTr->mRotationX = 90;

}

void Enemy::Update(float deltaTime)
{

	if (pBd)
		pBd->Update(deltaTime);
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = pTr->mPositionZ + 5;
	if (health < 0)
	{
		health = 0;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mScaleX = 2 * (health / 100);

	if (abs(pBd->mVelo_x) < 1 && abs(pBd->mVelo_y) < 1)
	{
		seek = true;
	}

	if (seek == true)
	{
		if (pBd != nullptr)
		{
			pSearchBody->mPos_x = pBd->mPos_x;
			pSearchBody->mPos_y = pBd->mPos_y;
			pSearchBody->mPos_z = pBd->mPos_z;

			Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
			if (pSearchBody != nullptr && b2 != nullptr)
			{
				if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(pSearchArea, pSearchBody->mPos_x, pSearchBody->mPos_y, pSearchBody->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
				{
					float dx, dy;
					dx = pBd->mPos_x - b2->mPos_x;
					dy = pBd->mPos_y - b2->mPos_y;
					float newRot = XMConvertToDegrees(atan2(dy, dx)) - 90;
					if (pBd->mPos_x < b2->mPos_x)
					{
						pBd->mVelo_x = 0.15f;
						if ((abs(pTr->mRotationZ - newRot) < 15) || (abs(pTr->mRotationZ - newRot) > 345))
						{
							pBd->mVelo_x = 0.80f;
						}
					}
					if (pBd->mPos_x > b2->mPos_x)
					{
						pBd->mVelo_x = -0.15f;
						if ((abs(pTr->mRotationZ - newRot) < 15) || (abs(pTr->mRotationZ - newRot) > 345))
						{
							pBd->mVelo_x = -0.80f;
						}
					}
					if (pBd->mPos_y < b2->mPos_y)
					{
						pBd->mVelo_y = 0.15f;
						if ((abs(pTr->mRotationZ - newRot) < 15) || (abs(pTr->mRotationZ - newRot) > 345))
						{
							pBd->mVelo_y = 0.80f;
						}
					}
					if (pBd->mPos_y > b2->mPos_y)
					{
						pBd->mVelo_y = -0.15f;
						if ((abs(pTr->mRotationZ - newRot) < 15) || (abs(pTr->mRotationZ - newRot) > 345))
						{
							pBd->mVelo_y = -0.80f;
						}
					}

					if ((abs(pTr->mRotationZ - newRot) < 15) || (abs(pTr->mRotationZ - newRot) > 345))
					{
						pTr->mRotationZ = newRot;
					}
					else if ((abs(pTr->mRotationZ - newRot) > 180))
					{
						if (pTr->mRotationZ > newRot)
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
					else if (pTr->mRotationZ > newRot)
					{
						pTr->mRotationZ -= 5;
					}
					else
					{
						pTr->mRotationZ += 5;
					}
					//pTr->mRotationZ = newRot;
				}
			}
		}
	}
}

void Enemy::CreateUI()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", curKey);
}


