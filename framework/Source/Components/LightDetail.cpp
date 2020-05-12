/**
 * @file LightDetail.cpp
 * @authors Pratyush Gawai
 * @date 9/21/19
 * @brief TThis file gives details about the light.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../pch.h"
#include "LightDetail.h"
#include "../../Manager/ResourceManager.h"
#include "..//..//Manager/MemoryManager.h"
#include "../Graphics/Light.h"
LightDetail::LightDetail() : Component(LIGHTDETAIL)
{

}

LightDetail::~LightDetail() {
	delete pLt;
}

void LightDetail::Update(float deltaTime) {}

void LightDetail::Serialize(Json::Value root, unsigned int key)
{
	Json::Value dataRoot = root["Type"];

	if (dataRoot.asString() == "DirectLight")
	{
		Light* pLt = new Light();
		this->pLt = pLt;
		pLt->SetType(DIRECT_LIGHT);

		dataRoot = root["Direction"];
		pLt->SetDirection(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Diffuse"];
		pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Ambient"];
		pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Specular"];
		pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	}
	else if (dataRoot.asString() == "PointLight")
	{
		Light* pLt = new Light();
		this->pLt = pLt;
		pLt->SetType(POINT_LIGHT);

		dataRoot = root["Diffuse"];
		pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Ambient"];
		pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Specular"];
		pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["setAttenuation"];
		pLt->SetAttenuation(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	}
	else if (dataRoot.asString() == "SpotLight")
	{
		Light* pLt = new Light();
		this->pLt = pLt;
		pLt->SetType(SPOT_LIGHT);

		dataRoot = root["Direction"];
		pLt->SetDirection(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Diffuse"];
		pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Ambient"];
		pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["Specular"];
		pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["setAttenuation"];
		pLt->SetAttenuation(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		dataRoot = root["setCutOff"];
		pLt->SetCutOff(dataRoot[0].asFloat(), dataRoot[1].asFloat());
	}

	dataRoot = root["HasShadow"];
	if (dataRoot.asBool())
	{
		pLt->HasShadow(dataRoot.asBool());
		dataRoot = root["ShadowMapSize"];
		pLt->ShadowMap().Initialize(dataRoot[0].asFloat(), dataRoot[1].asFloat());
	}


	(*MemoryManager::GetInstance()->ObjectPool)[key].pLd = this;
	
	this->key = key;
}

void LightDetail::DeSerialize(Json::Value& root)
{
	



}

void LightDetail::HandleEvent(Event* EventToHandle)
{

}
