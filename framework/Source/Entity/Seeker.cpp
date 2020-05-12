#include "../pch.h"
#include "Seeker.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "..//..//Manager/CollisionManager.h"
#include "Player.h"
Seeker::Seeker() : Entity(ENTITY_TYPE::SEEKER)
{
	pBd = nullptr;
	key = -1;
	health = 100.0f;
}

Seeker::~Seeker() {}

void Seeker::Serialize(Json::Value root, unsigned int position)
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Seeker";
	key = position;
	this->complete = true;
	this->runtime = false;
	//CreateUI();

}

void Seeker::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);


	for (int k = 0; k < ObjectFactoryManager::GetInstance()->LBoxPositions.size(); k++)
	{
		int sbox_pos = ObjectFactoryManager::GetInstance()->LBoxPositions.at(k);
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid((*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mShape,
			(*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_y,
			(*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_z, pBd->mShape, pBd->mPos_x, pBd->mPos_y, pBd->mPos_z))
		{
			if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid((*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mShape,
				(*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_y,
				(*MemoryManager::GetInstance()->ObjectPool)[sbox_pos].pBd->mPos_z, 
				(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mShape, (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x,
				(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y, (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_z))
			{
				Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
				if (player->spotcheck == true)
				{
					player_visible = true;
				}
				else
				{
					player_visible = false;
				}
			}
		}
		else
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pTr->mPositionX;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pTr->mPositionY;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = pTr->mPositionZ + 50;
		}
	}
	if (player_visible == true)
	{
		Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
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
	}
}
void Seeker::CreateUI()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", curKey);
}