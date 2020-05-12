/**
 * @file Body.cpp
 * @authors Vineet Amarjeet Dogra, Romil Tendulkar
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../pch.h"
#include "Body.h"
#include "Transform.h"
#include "../Entity/Pathfinder.h"
#include "../../Manager/MemoryManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "..\Util\SpatialGrid.h"



Body::Body() : Component(BODY)
{
	//initialize default values
	mIsPhysicsObject = false;
	mPos_x = mPos_y = mPos_z = 0;
	mScale_x = mScale_y = mScale_z = 1;
	mVelo_x = mVelo_z = 0;
	mVelo_y = 0.0;
	mAcc_x = mAcc_y = 0;
	mAcc_z = -5;
	mAng_x = mAng_y = mAng_z = 0;
	dOffset_x = dOffset_y = dOffset_z = 0;
	mMass = mInverseMass = 1;
	mShape = nullptr;
	active = true;
	key = -1;
	ShapeName = "";
}

Body::~Body()
{
	delete mShape;
}



void Body::Update(float deltaTime)
{
	if (deltaTime > 0.03) {
		return;
	}
	Integrate(deltaTime);
}
void Body::DeSerialize(Json::Value& root)
{
	root["Body"] = (Json::objectValue);
	root["Asset"]["ShapeX"] = ShapeName;

	Json::Value bScale(Json::arrayValue);
	bScale.append(mScale_x);
	bScale.append(mScale_y);
	bScale.append(mScale_z);
	root["Asset"]["BScale"] = bScale;

	Json::Value velocity(Json::arrayValue);
	velocity.append(mVelo_x);
	velocity.append(mVelo_y);
	velocity.append(mVelo_z);
	root["Asset"]["Velocity"] = velocity;

	Json::Value offset(Json::arrayValue);
	offset.append(dOffset_x);
	offset.append(dOffset_y);
	offset.append(dOffset_z);
	root["Asset"]["Offset"] = offset;

	root["Asset"]["IsPhysicsObject"] = mIsPhysicsObject;

	root["Asset"]["Mass"] = mMass;

	root["Asset"]["ShapeX"] = mFrictionOnSurface;

}

void Body::Serialize(Json::Value root, unsigned int key)
{
	etype = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn->GetEntityType();
	mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX - dOffset_x;
	mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionY - dOffset_y;
	mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionZ - dOffset_z;

	/*mScale_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleX/2;
	mScale_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleY/2;
	mScale_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mScaleZ/2;*/

	Json::Value dataRoot = root["ShapeX"];
	if (!dataRoot.isNull())
	{
		ShapeName = dataRoot.asString();
	}

	dataRoot = root["BScale"];
	if (!dataRoot.isNull())
	{

		mScale_x = dataRoot[0].asFloat() / 2;
		mScale_y = dataRoot[1].asFloat() / 2;
		mScale_z = dataRoot[2].asFloat() / 2;
	}
	dataRoot = root["Velocity"];
	if (!dataRoot.isNull())
	{

		mVelo_x = dataRoot[0].asFloat() / 2;
		mVelo_y = dataRoot[1].asFloat() / 2;
		mVelo_z = dataRoot[2].asFloat() / 2;
	}
	dataRoot = root["Offset"];
	if (!dataRoot.isNull())
	{

		dOffset_x = dataRoot[0].asFloat() / 2;
		dOffset_y = dataRoot[1].asFloat() / 2;
		dOffset_z = dataRoot[2].asFloat() / 2;
	}

	/*if (root.find(L"ShapeX") != root.end())
	{
		const std::wstring shapeType = root[L"ShapeX"]->AsString();
		shapeTypeName = std::string(shapeType.begin(), shapeType.end());
	}*/

	if (ShapeName == "AABB")
	{
		ShapeAABB* newShape = new ShapeAABB(this);

		newShape->mBottom = 0 - mScale_z;
		newShape->mTop = 0 + mScale_z;

		newShape->mIn = 0 + mScale_y;
		newShape->mOut = 0 - mScale_y;

		newShape->mLeft = 0 - mScale_x;
		newShape->mRight = 0 + mScale_x;
		if (mShape) delete mShape;
		mShape = newShape;
		mHeight = abs(newShape->mTop - newShape->mBottom) / 2;
		mWidth = abs(newShape->mLeft - newShape->mRight) / 2;
		mDepth = abs(newShape->mIn - newShape->mOut) / 2;
	}

	dataRoot = root["IsPhysicsObject"];
	if (!dataRoot.isNull())
	{
		bool Physics_Check = dataRoot.asBool();
		mIsPhysicsObject = Physics_Check;
	}
	dataRoot = root["Mass"];
	if (!dataRoot.isNull())
	{
		mMass = dataRoot.asFloat();
	}
	dataRoot = root["Friction"];
	if (!dataRoot.isNull())
	{
		mFrictionOnSurface = dataRoot.asFloat();
	}
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd = this;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
	this->key = key;


}

void Body::Integrate(float DT)
{
	//DT = 0;
	//std::cout << "DT IS " << DT << std::endl;
	if (DT < 0.05)
	{
		//std::cout << mVelo_y << std::endl;
		DT *= 1.5;
		if (mIsPhysicsObject)
		{
			/*
				mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX;
				mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionY;
				mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionZ;
			*/
			if (mVelo_z > 0)
			{
				DT *= 2;
			}
			if (abs(mVelo_x) > 5)
			{
				mAcc_x = 0;
			}
			mVelo_x = mVelo_x + (mAcc_x * DT);
			mVelo_y = mVelo_y + (mAcc_y * DT);
			if (body_State != ON_SURFACE)
			{
				mVelo_z = mVelo_z + (mAcc_z * DT);
			}

			//use this to affect the velocity curve


			if (body_State == ON_SURFACE && (mpEnowner->GetEntityType() == ENTITY_TYPE::PLAYER || mpEnowner->GetEntityType() == ENTITY_TYPE::COLLIDER || mpEnowner->GetEntityType() == ENTITY_TYPE::ENEMY ||
				mpEnowner->GetEntityType() == ENTITY_TYPE::WENEMY || mpEnowner->GetEntityType() == ENTITY_TYPE::CHARGER || mpEnowner->GetEntityType() == ENTITY_TYPE::SHOOTER ||
				mpEnowner->GetEntityType() == ENTITY_TYPE::BLOCK || mpEnowner->GetEntityType() == ENTITY_TYPE::PATHFINDER || mpEnowner->GetEntityType() == ENTITY_TYPE::BOSS1
				|| mpEnowner->GetEntityType() == ENTITY_TYPE::BOSS2
			 || mpEnowner->GetEntityType() == ENTITY_TYPE::GUIDEBOX || mpEnowner->GetEntityType() == ENTITY_TYPE::SWITCH || mpEnowner->GetEntityType() == ENTITY_TYPE::PILLAR))
			{
				mVelo_z = 0;
				
			}
			if ((mpEnowner->GetEntityType() == ENTITY_TYPE::VISIBLE_BOX))
			{
				mVelo_z = 0;
				mAcc_z = 0;
			}
			if (mAcc_z == 0)
			{
				mVelo_z = 0;
			}
			mPos_x = mPos_x + (mVelo_x * DT);
			mPos_y = mPos_y + (mVelo_y * DT);
			mPos_z = mPos_z + (mVelo_z * DT);

			//This code will refactored once we implement entities in their entirity.


			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX = mPos_x + dOffset_x;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionY = mPos_y + dOffset_y;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionZ = mPos_z + dOffset_z;

			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x = mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y = mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_z = mPos_z;

			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
		}
	}
}
void Body::HandleEvent(Event* EventToHandle)
{

}