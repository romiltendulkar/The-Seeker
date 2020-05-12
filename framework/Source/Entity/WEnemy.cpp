#include "../pch.h"
#include "WEnemy.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/TimerManager.h"
#include "../Entity/ViewBox.h"
#include "..//..//Manager/CollisionManager.h"

WEnemy::WEnemy() : Entity(ENTITY_TYPE::WENEMY)
{
	pBd = nullptr;
	pAn = nullptr;
	mTimer = 0;
	mTimerLimit = 5;
}

WEnemy::~WEnemy() {}

void WEnemy::Serialize(Json::Value root, unsigned int position)
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

	dataRoot = root["UpDown"];

	if (!dataRoot.isNull())
	{
		mGoingUp = dataRoot.asBool();
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "WEnemy";
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	this->runtime = false;
	key = position;
	for (int i = 0; i < 6; ++i)
	{
		int x = (i + 1);
		offset.push_back(x*x);
	}
	CreateViewCone();
	CreateSpotLight();
	
	++ObjectFactoryManager::GetInstance()->mUDct;
	if (ObjectFactoryManager::GetInstance()->mUDct % 2 == 0)
	{
		mGoingUp = true;
	}
	else
	{
		mGoingUp = false;
	}

}

void WEnemy::Update(float deltaTime)
{
	mTimer += TimerManager::GetInstance()->getDeltaTime();
	if (mTimer > mTimerLimit) {
		mGoingUp = !mGoingUp;
		mTimer = 0;
	}
	if (mGoingUp)
	{
		pTr->mPositionX += 0.1f;
		pBd->mPos_x = pTr->mPositionX;
	}
	else
	{
		pTr->mPositionX -= 0.1f;
		pBd->mPos_x = pTr->mPositionX;
	}
	if (pBd)
	{
		pBd->Update(deltaTime);
	}
	(*MemoryManager::GetInstance()->ObjectPool)[spos].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[spos].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[spos].pTr->mPositionZ = pTr->mPositionZ + 50;
	for (int i = 0; i < 6; ++i)
	{
		SetViewBoxPos(LVPos[i],i);
	}
	if (pAn)
		pAn->Update(deltaTime);
	//check collision check with player.
	for (unsigned int j = key + 1 ; j <= key + 6; j++)
	{
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid((*MemoryManager::GetInstance()->ObjectPool)[j].pBd->mShape, 
			(*MemoryManager::GetInstance()->ObjectPool)[j].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[j].pBd->mPos_y,
			(*MemoryManager::GetInstance()->ObjectPool)[j].pBd->mPos_z, (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mShape,
			(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y,
			(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_z))
		{
			//std::cout << "collision with player" << std::endl;
			colliding_player = true;
			colliding_player_position = j;
			break;
		}
	}
	can_see_player = false;
	if (colliding_player == true)
	{
		for (int i = 0; i < ObjectFactoryManager::GetInstance()->LWallPositions.size(); i++)
		{
			for (unsigned int k = key + 1; k <= key + 6; k++)
			{
				int wall_pos = ObjectFactoryManager::GetInstance()->LWallPositions.at(i);
				if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid((*MemoryManager::GetInstance()->ObjectPool)[k].pBd->mShape, 
					(*MemoryManager::GetInstance()->ObjectPool)[k].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[k].pBd->mPos_y, 
					(*MemoryManager::GetInstance()->ObjectPool)[k].pBd->mPos_z,(*MemoryManager::GetInstance()->ObjectPool)[wall_pos].pBd->mShape,
					(*MemoryManager::GetInstance()->ObjectPool)[wall_pos].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[wall_pos].pBd->mPos_y,
					(*MemoryManager::GetInstance()->ObjectPool)[wall_pos].pBd->mPos_z))
				{
					//std::cout << "collision with wall" <<std::endl;
					colliding_wall_position = k;
					wall_at = wall_pos;
					break;
				}
			}
		}
	}
	if (colliding_player_position != -1 && colliding_wall_position != -1)
	{
		if (colliding_player_position < colliding_wall_position)
		{
			can_see_player = true;
		//	std::cout << "can see player" << std::endl;
		}
		else if (colliding_player_position == colliding_wall_position)
		{
			if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x < (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x < (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x)
			{
			//	std::cout << "can see player" << std::endl;
				can_see_player = true;
			}
			if((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x < (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x && (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x < (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x)
			{
				can_see_player = false;
			//	std::cout << "cannot see player" << std::endl;
			}
			if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x > (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x && (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x > (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x)
			{
				can_see_player = true;
			//	std::cout << "can see player" << std::endl;
			}
			if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x > (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x&& (*MemoryManager::GetInstance()->ObjectPool)[wall_at].pBd->mPos_x > (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x)
			{
				can_see_player = false;
			//	std::cout << "cannot see player" << std::endl;
			}
		}
		else
		{
			can_see_player = false;
			//std::cout << "cannot see player" << std::endl;
		}
	}
	else if (colliding_player_position != -1 && colliding_wall_position ==-1)
	{
		//std::cout << "can see player" << std::endl;
		can_see_player = true;
	}

	if (can_see_player == true)
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

	colliding_player_position = -1;
	colliding_wall_position = -1;
	wall_at = -1;
	colliding_player = false;
	can_see_player = false;
}

void WEnemy::CreateViewCone()
{
	int curKey = MemoryManager::GetInstance()->current_position;
	float x = 0.5;
	for (int i = 1; i <= 6; i++)
	{
		ObjectFactoryManager::GetInstance()->LoadObject("ViewBox", ++curKey);
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pAs->obj->SetDiffuse(1.0f, 1.0f, 0.9f);
		ViewBox* vb = static_cast<ViewBox*>((*MemoryManager::GetInstance()->ObjectPool)[curKey].pEn);
		vb->ownerEnemy = key;
		//(*MemoryManager::GetInstance()->ObjectPool)[curKey].pAs->obj->SetSpecular(1,1, 1);
		/*(*MemoryManager::GetInstance()->ObjectPool)[curKey].pAs->obj->SetShininess(1);*/
		SetViewBoxDim(curKey, int(x), i);
		LVPos.push_back(curKey);
		x = x + 0.5f;
	}
}

void WEnemy::CreateSpotLight()
{
	int curKey = MemoryManager::GetInstance()->current_position;
	++curKey;
	ObjectFactoryManager::GetInstance()->LoadObject("SpotLight", curKey);
	spos = curKey;
}

void WEnemy::SetViewBoxDim(int key, int dimx,int dimy)
{
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x = float(dimx);
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y = float(dimy);
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z = 0;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX = float(dimx*2);
	(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY = float(dimy*2);
	(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleZ = float(0);
	ShapeAABB* newShape = new ShapeAABB((*MemoryManager::GetInstance()->ObjectPool)[key].pBd);

	newShape->mBottom = 0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z;
	newShape->mTop = 0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_z;

	newShape->mIn = 0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y;
	newShape->mOut = 0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_y;

	newShape->mLeft = 0 - (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x;
	newShape->mRight = 0 + (*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mScale_x;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mShape = newShape;
}

void WEnemy::SetViewBoxPos(int key1,int i)
{
	
	//int offset = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY/3;
	int off = offset[i]/5;
	
	if (mGoingUp)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionX = pTr->mPositionX + off;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionY = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionZ = pTr->mPositionZ;

		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_x = pTr->mPositionX + off;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_y = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_z = pTr->mPositionZ;
	}
	else
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionX = pTr->mPositionX - off;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionY = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pTr->mPositionZ = pTr->mPositionZ;

		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_x = pTr->mPositionX - off;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_y = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key1].pBd->mPos_z = pTr->mPositionZ;
	}
	
}
