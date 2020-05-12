/**
 * @file Transform.h
 * @authors Vineet Amarjeet Dogra, Pratyush Gawai
 * @date 10/1/19
 * @brief This file governs how the transform component is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../pch.h"
#include "Component.h"
class Transform : public Component
{
public:
	Transform();
	~Transform();
	
	//void Serialize(JSONObject root);
	void Update(float deltaTime);

	/**
		Serializes transform
		Root : JSON Root
		key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int key);

	/**
		DeSerialize transform
	*/
	void DeSerialize(Json::Value& root);

	void HandleEvent(Event *EventToHandle);
	
public:

	Vector3 translate;

	float mPositionX;
	float mPositionY;
	float mPositionZ;

	float mScaleX;
	float mScaleY;
	float mScaleZ;

	float mRotationX;
	float mRotationZ;
	//bool assigned = false;

	unsigned int key;
};

#endif