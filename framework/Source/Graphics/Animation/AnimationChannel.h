/**
 * @file AnimationChannel.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Channel for animations
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
//#define _HAS_STD_BYTE 0
#include "../../pch.h"
#include "../../Util/Math.h"

using namespace std;
struct PositionKey
{
	PositionKey(double in_time, Vector3 in_V)
	{
		Time = in_time;
		V = in_V;
	}
	double Time;
	Vector3 V;
};

struct RotationKey
{
	RotationKey(double in_time, MyQuaternion& in_Q)
	{
		Time = in_time;
		Q = in_Q;
	}
	double Time;
	MyQuaternion Q;
};
struct ScalingKey
{
	ScalingKey(double in_time, Vector3 in_S)
	{
		Time = in_time;
		S = in_S;
	}
	double Time;
	Vector3 S;
};
class AnimationChannel
{
public:
	AnimationChannel();
	~AnimationChannel();
	
	/**
	Adds position key
	*/
	void AddPositionKey(double time, float v0, float v1, float v2);

	/**
		Adds rotation key
	*/
	void AddRotationKey(double time, float v0, float v1, float v2, float v3);
	
	/**
		Adds addScaling key
	*/
	void AddScalingKey(double time, float v0, float v1, float v2);
	
	/**
		Interpolates the animation channel
	*/
	void Interpolate(float t);

	Matrix& GetFinalInterpolate();
	Vector3& GetFinalTranslation();
	MyQuaternion& GetFinalRotation();
	Vector3& GetFinalScale();


	Matrix FinalInterpolate;
	Vector3 FinalTranslation;
	MyQuaternion FinalRotation;
	Vector3 FinalScale;

private:
	vector<PositionKey> vlist;
	vector<RotationKey> qlist;
	vector<ScalingKey> slist;
	UINT mVFoundIndex = 0;
	UINT mQFoundIndex = 0;
	UINT mSFoundIndex = 0;
	template <typename T>
	UINT BinarySearch(T& list, float t,UINT index);

};
