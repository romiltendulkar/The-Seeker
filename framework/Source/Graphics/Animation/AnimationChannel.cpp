/**
 * @file AnimationChannel.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Channel for animations
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#include "../../pch.h"
#include "AnimationChannel.h"
#include "../../Util/Math.h"

AnimationChannel::AnimationChannel()
{
}


AnimationChannel::~AnimationChannel()
{
}

void AnimationChannel::AddPositionKey(double time, float v0, float v1, float v2)
{
	vlist.push_back(PositionKey(time, Vector3(v0, v1, v2)));
}

void AnimationChannel::AddRotationKey(double time, float v0, float v1, float v2, float v3)
{
	qlist.push_back(RotationKey(time, MyQuaternion(v3, v0, v1, v2)));
}

void AnimationChannel::AddScalingKey(double time, float v0, float v1, float v2)
{
	slist.push_back(ScalingKey(time, Vector3(v0, v1, v2)));
}

void AnimationChannel::Interpolate(float t)
{
	Vector3 p;
	MyQuaternion r;
	Vector3 s;
	if (t <= vlist.front().Time)
	{
		p = vlist.front().V;
	}
	else if (t >= vlist.back().Time)
	{
		p = vlist.back().V;
	}
	else if (t >= vlist[mVFoundIndex].Time&&t <= vlist[mVFoundIndex + 1].Time)
	{
		float newt = (float)(t - vlist[mVFoundIndex].Time);
		p = TranslationLERP(vlist[mVFoundIndex].V, vlist[mVFoundIndex + 1].V, newt);
	}
	else
	{
		mVFoundIndex = BinarySearch(vlist, t, 0);
		float newt = (float)(t - vlist[mVFoundIndex].Time);
		p = TranslationLERP(vlist[mVFoundIndex].V, vlist[mVFoundIndex +1].V, newt);
	}

	if (t <= qlist.front().Time)
	{
		r = qlist.front().Q;
	}
	else if (t >= qlist.back().Time)
	{
		r = qlist.back().Q;
	}
	else if (t >= qlist[mQFoundIndex].Time&&t <= qlist[mQFoundIndex + 1].Time)
	{
		float newt = (float)(t - qlist[mQFoundIndex].Time);
		r = MyQuaternion::Slerp(newt, qlist[mQFoundIndex].Q, qlist[mQFoundIndex+1].Q);
	}
	else
	{
		mQFoundIndex = BinarySearch(qlist, t, 0);
		float newt = (float)(t - qlist[mQFoundIndex].Time);
		r = MyQuaternion::Slerp(newt, qlist[mQFoundIndex].Q, qlist[mQFoundIndex +1].Q);
	}

	if (t <= slist.front().Time)
	{
		s = slist.front().S;
	}
	else if (t >= slist.back().Time)
	{
		s = slist.back().S;
	}
	else if (t >= slist[mSFoundIndex].Time&&t <= slist[mSFoundIndex + 1].Time)
	{
		float newt = (float)(t - slist[mSFoundIndex].Time);
		s = ScaleLERP(slist[mSFoundIndex].S, slist[mSFoundIndex + 1].S, newt);
	}
	else
	{
		mSFoundIndex = BinarySearch(slist, t, 0);
		float newt = (float)(t - slist[mSFoundIndex].Time);
		s = ScaleLERP(slist[mSFoundIndex].S, slist[mSFoundIndex + 1].S, newt);
	}
	Matrix posM = Matrix::CreateTranslation(p).Transpose();
	Matrix rotationM = r.ToMatrix();
	Matrix scaM = Matrix::CreateScale(s);
	FinalInterpolate = posM* rotationM*scaM;

	FinalTranslation= p;
	FinalRotation= r;
	FinalScale= s;
}

Matrix & AnimationChannel::GetFinalInterpolate()
{
	return FinalInterpolate;
}

Vector3 & AnimationChannel::GetFinalTranslation()
{
	return FinalTranslation;
}

MyQuaternion & AnimationChannel::GetFinalRotation()
{
	return FinalRotation;
}

Vector3 & AnimationChannel::GetFinalScale()
{
	return FinalScale;
}



template<typename T>
UINT AnimationChannel::BinarySearch(T& list, float t, UINT index)
{
	UINT frontIndex = index;
	UINT backIndex = list.size();
	while (backIndex - frontIndex > 1)
	{
		int m = frontIndex + (backIndex - frontIndex) / 2;

		if (list[m].Time == t)
		{
			frontIndex = m;
			backIndex = m + 1;
			break;
		}
		else if (list[m].Time > t)
		{
			backIndex = m;
		}
		else 
		{
			frontIndex = m;
		}
	}
	return frontIndex;
}
