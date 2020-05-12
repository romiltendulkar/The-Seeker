/**
 * @file BoneNode.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Updates all animations.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../../pch.h"
#include "BoneNode.h"
BoneNode::BoneNode(int in_index, string in_name, Matrix bindLocalTransform)
	:mIndex(in_index), mName(in_name), mLocalBindTransform(bindLocalTransform)
{
}

BoneNode::~BoneNode()
{
}

void BoneNode::addChild(BoneNode child)
{
	LChildren.push_back(child);
}

Matrix & BoneNode::getAnimatedTransform()
{
	return mLocalBindTransform;
}

void BoneNode::setAnimationTransform(Matrix& animationTransform)
{
	mAnimatedTransform = animationTransform;
}

Matrix & BoneNode::getInverseBindTransform()
{
	return mInverseBindTransform;
}

Matrix & BoneNode::getGlobalBindTransform()
{
	return mGlobalBindTransform;
}
Matrix & BoneNode::GetLocalBindTransform()
{
	return mLocalBindTransform;
}

void BoneNode::calcInverseBindTransform(Matrix& parentBindTransform)
{
	Matrix bindTransform = parentBindTransform * mLocalBindTransform;
	mGlobalBindTransform = bindTransform;
	mInverseBindTransform = bindTransform.Invert();
	for (int i = 0; i < LChildren.size(); i++)
	{
		LChildren[i].calcInverseBindTransform(bindTransform);
	}
}
