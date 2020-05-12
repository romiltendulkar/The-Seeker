/**
 * @file BoneNode.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Updates all animations.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#pragma once
//#define _HAS_STD_BYTE 0
#include "../../pch.h"
using namespace std;
class BoneNode
{
public:
	BoneNode() {}
	BoneNode(int index, string name, Matrix bindLocalTransform);
	~BoneNode();

	/**
		Adds child animations
	*/
	void addChild(BoneNode child);

	/**
		Gets animated Transform
	*/
	Matrix& getAnimatedTransform();

	/**
		Sets animation Transform
	*/
	void setAnimationTransform(Matrix& animationTransform);

	/**
		Gets inverse bind transform
	*/
	Matrix& getInverseBindTransform();

	/**
		Gets global bind transform
	*/
	Matrix & getGlobalBindTransform();

	/**
		Gets Local bind transform
	*/
	Matrix & GetLocalBindTransform();

	/**
	Calculates bind transform
	*/
	void calcInverseBindTransform(Matrix& parentBindTransform);
public:
	int mIndex;
	string mName;
	vector<BoneNode> LChildren;
private:
	Matrix mAnimatedTransform;
	Matrix mLocalBindTransform;
	Matrix mGlobalBindTransform;
	Matrix mInverseBindTransform;
};

