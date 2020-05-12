/**
 * @file Transform.cpp
 * @authors Vineet Amarjeet Dogra, Pratyush Gawai
 * @date 9/21/19
 * @brief This file governs how the transform component is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../pch.h"
#include "Transform.h"
#include "../../Manager/ResourceManager.h"
#include "..//..//Manager/MemoryManager.h"
Transform::Transform() : Component(TRANSFORM)
{
	mPositionX = mPositionY = mPositionZ = 0.0f;
	mScaleX = mScaleY = mScaleZ = 0.0f;
	mRotationX = 0;
	mRotationZ = 0;
	key = -1;
}

Transform::~Transform() {}

void Transform::Update(float deltaTime) {}

void Transform::Serialize(Json::Value root, unsigned int key)
{
	Json::Value dataRoot = root["Translate"];
	if (!dataRoot.isNull())
	{
		mPositionX = dataRoot[0].asFloat();
		mPositionY = dataRoot[1].asFloat();
		mPositionZ = dataRoot[2].asFloat();
	}
	dataRoot = root["Scale"];
	if (!dataRoot.isNull())
	{
		mScaleX = dataRoot[0].asFloat();
		mScaleY = dataRoot[1].asFloat();
		mScaleZ = dataRoot[2].asFloat();
	}


	(*MemoryManager::GetInstance()->ObjectPool)[key].pTr = this;
	//(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
	this->key = key;
}

void Transform::DeSerialize(Json::Value& root)
{
	root["Transform"] = (Json::objectValue);

	Json::Value translate(Json::arrayValue);
	translate.append(mPositionX);
	translate.append(mPositionY);
	translate.append(mPositionZ);
	root["Transform"]["Translate"] = translate;

	Json::Value scale(Json::arrayValue);
	scale.append(mScaleX);
	scale.append(mScaleY);
	scale.append(mScaleZ);
	root["Transform"]["Scale"] = scale;

}

void Transform::HandleEvent(Event* EventToHandle)
{

}
