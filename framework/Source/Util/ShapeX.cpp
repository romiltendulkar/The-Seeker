/**
 * @file ShapeX.h
 * @author Romil Tendulkar
 * @date 10/22/2019
 * @brief This file implements the shape classes that are used for collision detection
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "ShapeX.h"
#include "../Components/Body.h"
ShapeX::ShapeX()
{
}

/**
	Initializes the type and the owner body of the shape

	\param pOwner | Pointer to the body that the shape is initialized for
	\param ptype | enum for the type of shape
*/
ShapeX::ShapeX(Body* pOwner,SHAPE_TYPE ptype)
{
	type = ptype;
	pOwnerBody = pOwner;
}

/**
	initializes a AABB shape object

	\param pOwner | Pointer to the body that the shape is initialized for
*/
ShapeAABB::ShapeAABB(Body* pOwner) : ShapeX(pOwner,SHAPE_AABB)
{
	mTop = mBottom = mLeft = mRight = mOut = mIn = 0;
}

ShapeAABB::ShapeAABB()
{
}

ShapeAABB::~ShapeAABB()
{

}


std::pair<bool, float> RayCastAABB(Vector3 startp, Vector3 dir, ShapeAABB & aabb)
{
	Vector3 s[3];
	s[0] = Vector3(1, 0, 0);
	s[1] = Vector3(0, 1, 0);
	s[2] = Vector3(0, 0, 1);
	float x = aabb.pOwnerBody->mPos_x;
	float y = aabb.pOwnerBody->mPos_y;
	float z = aabb.pOwnerBody->mPos_z;

	float d0[3] = { -(x+aabb.mLeft),-(y+aabb.mOut) ,-(z+aabb.mBottom) };
	float d1[3] = { -(x+aabb.mRight),-(y+aabb.mIn) ,-(z+aabb.mTop) };
	float Ft0 = 0;
	float Ft1 = FLT_MAX;
	float t0 = 0;
	float t1 = FLT_MAX;
	for (size_t i = 0; i < 3; i++)
	{
		if (s[i].Dot(dir))
		{
			float NdotQ = s[i].Dot(startp);
			float NdotD = s[i].Dot(dir);
			t0 = -(d0[i] + NdotQ) / NdotD;
			t1 = -(d1[i] + NdotQ) / NdotD;
			if (t0 > t1) {
				std::swap(t0, t1);
			}
			Ft0 = max(Ft0, t0);
			Ft1 = min(Ft1, t1);
		}
		else
		{
			float NdotQ = s[i].Dot(startp);
			float s0 = NdotQ + d0[i];
			float s1 = NdotQ + d1[i];
			if (s0 * s1 < 0.0f)
			{
				Ft0 = max(Ft0, 0);
				Ft1 = min(Ft1, FLT_MAX);
			}
			else
			{
				Ft0 = max(Ft0, 1);
				Ft1 = min(Ft1, 0);
			}
		}
	}
	if (Ft0 > Ft1)
	{
		return { false,0 };
	}
	else//(Ft0 <= Ft1)
	{
		if (Ft0 > 0)
		{
			return { true,Ft0 };
		}
		if (Ft1 > 0)
		{
			return { true,Ft1 };
		}
	}
	return { false,0 };
}
