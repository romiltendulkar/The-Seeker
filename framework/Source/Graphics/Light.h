/**
 * @file Light.h
 * @author Ching-Yen Lin
 * @date 10/10/19
 * @brief This file content Light member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef LIGHT_H
#define LIGHT_H
#include "../pch.h"
#include "ShadowShading\Shadow.h"
enum LIGHTTYPE
{
	DIRECT_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};
struct LightInform
{
	Matrix LightProjView1;
	Matrix LightProjView2;
	Matrix LightProjView3;
	Matrix LightProjView4;
};
struct LightInfo
{
	Vector4 attribute;
	Vector3 position;
	float cutOff;
	Vector3 direction;
	float outerCutOff;
	Vector3 ambient;
	float constant;
	Vector3 diffuse;
	float linear;
	Vector3 specular;
	float quadratic;
	Matrix shadowMartix;
};
class Light
{
public:
	/**
   initial all member to zero
	*/
	Light();
	/**
	copy data
	 */
	Light(LightInfo);
	~Light();
	/**
	Set Position
	 */
	void SetPosition(float, float, float);

	/**
	Set Direction
	 */
	void SetDirection(float, float, float);

	/**
	Set Ambient
	 */
	void SetAmbient(float, float, float);

	/**
	Set Diffuse
	 */
	void SetDiffuse(float, float, float);

	/**
	Set Specular
	 */
	void SetSpecular(float, float, float);

	/**
	Set Attenuation
	 */
	void SetAttenuation(float, float, float);

	/**
	Set CutOff
	 */
	void SetCutOff(float, float);

	/**
	Set Enable
	 */
	void SetEnable(bool);

	/**
	Set Type
	 */
	void SetType(LIGHTTYPE);

	/**
	Get Light View Matrix
	 */
	Matrix& GetLightViewMatrix();

	/**
	Get Light Projection Matrix
	 */
	Matrix& GetLightProjectionMatrix();

	/**
	Set ShadowMap size
	 */
	void ShadowMap(float, float);

	/**
	get ShadowMap
	 */
	Shadow& ShadowMap();

	/**
	set Shadow
	 */
	void HasShadow(bool);
	void ChangeSize(float);
	/**
	Has Shadow?
	 */
	bool HasShadow();

	/**
	get LightInfo
	 */
	LightInfo& GetInfo();
	Matrix mLightViewMatrix;
	Matrix mLightProjectionMatrix;

private:
	void ComputerShadowMatrix();
	bool mHasShadow;
	Shadow mShadowMap;
	LightInfo mInfo;
	float mSize=1;
};

#endif 