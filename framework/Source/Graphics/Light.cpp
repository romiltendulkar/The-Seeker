/**
 * @file Light.cpp
 * @author Ching-Yen Lin
 * @date 10/10/19
 * @brief This file content Light function like set member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Light.h"
#include "../../../Manager/GraphicsManager.h"
#include "../Util/Math.h"

 /**
initial all member to zero
 */
Light::Light()
{
	mHasShadow = false;
	mInfo = { Vector4(),Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Matrix() };
	mInfo.shadowMartix._44 = -1.0f;
}
/**
copy data
 */
Light::Light(LightInfo info)
{
	mInfo = info;
	mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}


Light::~Light()
{
}
/**
Set Position
 */
void Light::SetPosition(float x, float y, float z)
{
	mInfo.position.x = x;
	mInfo.position.y = y;
	mInfo.position.z = z;
	//mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}
/**
Set Direction
 */
void Light::SetDirection(float dirX, float dirY, float dirZ)
{
	mInfo.direction = MyNormalize(Vector3(dirX, dirY, dirZ));

	//mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}
/**
Set Ambient
 */
void Light::SetAmbient(float r, float g, float b)
{
	mInfo.ambient.x = r;
	mInfo.ambient.y = g;
	mInfo.ambient.z = b;
}
/**
Set Diffuse
 */
void Light::SetDiffuse(float r, float g, float b)
{
	mInfo.diffuse.x = r;
	mInfo.diffuse.y = g;
	mInfo.diffuse.z = b;
}
/**
Set Specular
 */
void Light::SetSpecular(float r, float g, float b)
{
	mInfo.specular.x = r;
	mInfo.specular.y = g;
	mInfo.specular.z = b;
}
/**
Set Attenuation
 */
void Light::SetAttenuation(float c, float l, float q)
{
	mInfo.constant = c;
	mInfo.linear = l;
	mInfo.quadratic = q;
}
/**
Set CutOff
 */
void Light::SetCutOff(float cut, float othercut)
{
	mInfo.cutOff = cos(cut* DEGREE_TO_RADIAN);
	mInfo.outerCutOff = cos(othercut* DEGREE_TO_RADIAN);
}
/**
Set Enable
 */
void Light::SetEnable(bool enable)
{
	mInfo.attribute.y = enable;
}
/**
Set Type
 */
void Light::SetType(LIGHTTYPE type)
{
	mInfo.attribute.x = float(type);
}
/**
Get Light View Matrix
 */
Matrix& Light::GetLightViewMatrix()
{
	mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, (mInfo.position + mInfo.direction), Vector3(0, 1, 0));
	if (mInfo.attribute.x == DIRECT_LIGHT)
	{
		mLightViewMatrix = Matrix::CreateLookAt(mInfo.position -mInfo.direction * 500, mInfo.position, Vector3(0, 1, 0));
	}
	return mLightViewMatrix;
}
void Light::ChangeSize(float s)
{
	mSize = s;
}
/**
Get Light Projection Matrix
 */
Matrix& Light::GetLightProjectionMatrix()
{
	float light_dist = 500.0f;
	mLightProjectionMatrix = MyPerspective(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);
	if (mInfo.attribute.x == DIRECT_LIGHT)
	{
		//mLightProjectionMatrix = Matrix::CreateOrthographic(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f); /*(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);*/
		//mLightProjectionMatrix = Matrix::CreateOrthographic(40.0f / light_dist, 40.0f / light_dist, 1.0f, 100.0f).Transpose();
		mLightProjectionMatrix = Matrix::CreateOrthographic(mSize, mSize, 1.0f, 90.0f).Transpose();
		//mLightProjectionMatrix = Matrix::CreateOrthographicOffCenter(-1, 1, -1, 1, 1.0f, 10.0f).Transpose();
	}

	return mLightProjectionMatrix;
}
/**
Initialize Shadow Map
 */
void Light::ShadowMap(float rwidth, float rheight)
{
	mShadowMap.Initialize(rwidth, rheight);
}
/**
get Shadow Map
 */
Shadow & Light::ShadowMap()
{
	return mShadowMap;
}
/**
set hasShadow
 */
void Light::HasShadow(bool rhasshadow)
{
	mHasShadow = rhasshadow;
}
/**
get HasShadow
 */
bool Light::HasShadow()
{
	return mHasShadow;
}
/**
Get light Info
 */
LightInfo& Light::GetInfo()
{
	if (mHasShadow)
	{
		ComputerShadowMatrix();
	}
	return mInfo;
}
/**
Computer Shadow Matrix
 */
void Light::ComputerShadowMatrix()
{
	mInfo.shadowMartix = MyTranslate(0.5, 0.5, 0.5)*MyScale(0.5, 0.5, 0.5)*mLightProjectionMatrix.Transpose()*mLightViewMatrix.Transpose();

	if (mInfo.attribute.x == DIRECT_LIGHT)
	{
		//mLightProjectionMatrix = Matrix::CreateOrthographic(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f); /*(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);*/
		mInfo.shadowMartix = MyTranslate(0.5, 0.5, 0.5)*MyScale(0.5, 0.5, 0.5)*mLightProjectionMatrix.Transpose()*mLightViewMatrix.Transpose();
	}
}
