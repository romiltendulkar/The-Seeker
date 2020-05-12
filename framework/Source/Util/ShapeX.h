/**
 * @file ShapeX.h
 * @author Romil Tendulkar
 * @date 10/22/2019
 * @brief This file implements the shape classes that are used for collision detection
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
class Body;

enum SHAPE_TYPE
{
	SHAPE_AABB,
	SHAPE_CIRCLE
};

class ShapeX
{
public:
	ShapeX();
	ShapeX(Body* ,SHAPE_TYPE);
	~ShapeX(){};
	void SetBody(Body* body)
	{
		pOwnerBody = body;
	}
	Body* pOwnerBody;
	SHAPE_TYPE type;
};

class ShapeAABB : public ShapeX
{
public:
	ShapeAABB();
	ShapeAABB(Body* owner);
	~ShapeAABB();
	float mTop, mBottom, mLeft, mRight, mOut, mIn;
private:

};
std::pair<bool, float>RayCastAABB(Vector3 startp, Vector3 dir, ShapeAABB&);
