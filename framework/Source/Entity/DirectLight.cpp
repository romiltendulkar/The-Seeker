#include "../pch.h"
#include "DirectLight.h"
#include "../Components/Transform.h"
#include "../Components/LightDetail.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../Graphics/Light.h"

DirectLight::DirectLight() : Entity(ENTITY_TYPE::DIRECT_LIGHT)
{
}

DirectLight::~DirectLight() {}

void DirectLight::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	dataRoot = root["LightDetail"];

	if (!dataRoot.isNull())
	{
		LightDetail* pLd = new LightDetail();
		pLd->Serialize(dataRoot, position);
		this->pLd = pLd;

	}
	//dataRoot = root["Type"];
	//if (dataRoot.asString() == "DirectLight")
	//{
	//	Light* pLt = new Light();
	//	this->pLt = pLt;
	//	pLt->SetType(DIRECT_LIGHT);

	//	dataRoot = root["Direction"];
	//	pLt->SetDirection(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Diffuse"];
	//	pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	//	
	//	dataRoot = root["Ambient"];
	//	pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	//	
	//	dataRoot = root["Specular"];
	//	pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	//}
	//else if (dataRoot.asString() == "PointLight")
	//{
	//	Light* pLt = new Light();
	//	this->pLt = pLt;
	//	pLt->SetType(POINT_LIGHT);

	//	dataRoot = root["Diffuse"];
	//	pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Ambient"];
	//	pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Specular"];
	//	pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["setAttenuation"];
	//	pLt->SetAttenuation(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	//}
	//else if (dataRoot.asString() == "DirectLight")
	//{
	//	Light* pLt = new Light();
	//	this->pLt = pLt;
	//	pLt->SetType(SPOT_LIGHT);

	//	dataRoot = root["Direction"];
	//	pLt->SetDirection(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Diffuse"];
	//	pLt->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Ambient"];
	//	pLt->SetAmbient(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["Specular"];
	//	pLt->SetSpecular(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

	//	dataRoot = root["setAttenuation"];
	//	pLt->SetAttenuation(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());
	//	
	//	dataRoot = root["setCutOff"];
	//	pLt->SetCutOff(dataRoot[0].asFloat(), dataRoot[1].asFloat());
	//}

	//dataRoot = root["HasShadow"];
	//if (dataRoot.asBool())
	//{
	//	pLt->HasShadow(dataRoot.asBool());
	//	dataRoot = root["ShadowMapSize"];
	//	pLt->ShadowMap().Initialize(dataRoot[0].asInt(), dataRoot[1].asInt());
	//}

	pLd->pLt->SetPosition(pTr->mPositionX, pTr->mPositionY, pTr->mPositionZ);
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "DirectLight";
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->complete = true;
	key = position;
	this->runtime = false;
}

void DirectLight::Update(float deltaTime)
{
	if (pLd->pLt)
	{
		pLd->pLt->SetPosition(pTr->mPositionX, pTr->mPositionY, pTr->mPositionZ);
	}

}
