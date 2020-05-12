/**
 * @file Body.h
 * @authors Vineet Amarjeet Dogra, Romil Tendulkar
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#ifndef BODY_H
#define BODY_H
#endif // !BODY_H
#include "../pch.h"
#include "Component.h"
#include "..\Util\ShapeX.h"
#include "../Entity/Entity.h"

enum BODY_STATE
{
	FALLING,
	ON_SURFACE
};

//class Entity;
//class ShapeX;
class Body : public Component
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Body();
	~Body();

	/**
	   In this function we call the integrate function that governs what happens in between grames to the body.

	  \param rdeltaTime | Delta time between frames.
	*/
	void Update(float deltaTime);
	void DeSerialize(Json::Value& root);

	/**
	   In this function we serialize the body, we give it scale, transform, shape for OOBB, if it is a physics object or not and so on.

	  \param  root is used to retreieve value form json file, key is used to pinpoint which node in the memory manager we are working on.
	*/
	void Serialize(Json::Value root, unsigned int key);

	/**
	   In this function we tell the body what will happen to it in the give fra,e, how far it will fall or move based on what state it is in and what kind of physics object it is.

	  \param rdeltaTime | DT time between frames.
	*/
	void Integrate(float dT);

	void HandleEvent(Event* EventToHandle);

	//ENTITY_TYPE GetEntityType();
	//void ResetPosAndShape(Transform* pTr);

	BODY_STATE body_State = FALLING;
	bool mRightMovement = true;
	bool mLeftMovement = true;
	bool mDownMovement = true;
	bool mUpMovement = true;
	bool mCollidingSideRight = false;
	bool mCollidingSideLeft = false;
	bool mCollidingSideIn = false;
	bool mFacingRight = false;
	bool mFacingLeft = false;
	bool mFacingUp = true;
	bool mFacingDown = false;
	bool mCollidingSideOut = false;
	bool objectSelected = false;
	bool locked = false;

	float mHeight, mWidth, mDepth;

	bool mIsPhysicsObject;
	float mPos_x, mPos_y, mPos_z;
	float mVelo_x, mVelo_y, mVelo_z;
	float mScale_x, mScale_y, mScale_z;
	float mAng_x, mAng_y, mAng_z;
	float mMass;
	float mInverseMass;
	float gravity;
	float mFrictionOnSurface;
	float mAcc_x, mAcc_y, mAcc_z;
	float dOffset_x, dOffset_y, dOffset_z;
	ShapeX* mShape;
	ENTITY_TYPE etype;
	std::string ShapeName;
	unsigned int key;
	int mFrame_count = 0;
	bool active;
};