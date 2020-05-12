/**
 * @file Asset.cpp
 * @authors Vineet Amarjeet Dogra, Pratyush Gawai
 * @date 10/1/19
 * @brief This file governs how the asset component is serialized and is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Asset.h"

#include "../../Manager/ResourceManager.h"
#include"../../Manager/GraphicsManager.h"
#include"../../Manager/MemoryManager.h"
Asset::Asset() : Component(ASSET)
{
	obj = nullptr;
	tshape = nullptr;
	ShapeName = ModelName = TextureName="";
	key = -1;
	isRotated = false;
}

Asset::~Asset()
{
	if(obj)
		delete obj;
	//if (tshape)
	//	delete tshape;
	//if (tex)
	//	delete tex;
	//if (texN)
	//	delete texN;
}

void Asset::Update(float deltaTime)
{
}

void Asset::Serialize(Json::Value root, unsigned int key)
{
	Json::Value dataRoot = root["Shape"];
	if (!dataRoot.isNull())
	{
		ShapeName = dataRoot.asString();
	}
	if (ShapeName == "Obj" || ShapeName == "FBX")
	{
		//THESE 3 MUST BE THERE IF ITS AN OBJ OR FBX
		//1. Model : Cause obviously
		//2. Rotated : Because some models are rotated by 90 and there is no actual way to know unless its actually loaded somewhere
		//3. MaterialID : For blending, -1 = Debug, 0 = Normal Object, 1 = Skydome, 2 = for blending
		dataRoot = root["Model"];
		ModelName = dataRoot.asString();
		dataRoot = root["Rotated"];
		isRotated = dataRoot.asBool();
		dataRoot = root["MaterialID"];
		materialID = dataRoot.asInt(); 
	}
	
	Vector2 texsize = Vector2(1, 1);
	bool changesize=0;
	dataRoot = root["TextureResize"];
	if (!dataRoot.isNull())
	{
		texsize = Vector2(dataRoot[0].asFloat(), dataRoot[1].asFloat());
	}
	dataRoot = root["TextureResizeMode"];
	if (!dataRoot.isNull())
	{
		changesize = dataRoot.asBool();

	}
	dataRoot = root["Texture"];
	if(!dataRoot.isNull())
	{
		TextureName = dataRoot.asString();
		tex = ResourceManager::GetInstance()->LoadTexture(TextureName);
		tex->SetTextureSize(texsize);
		tex->SetTextureResizeMode(changesize);
	}
	dataRoot = root["NormTexture"];
	if (!dataRoot.isNull())
	{
		TextureName = dataRoot.asString();
		texN = ResourceManager::GetInstance()->LoadTexture(TextureName);
		texN->SetTextureSize(texsize);
		texN->SetTextureResizeMode(changesize);
	}

	tshape = ResourceManager::GetInstance()->LoadShape(ShapeName, ModelName);
	if (ShapeName == "Particles")
	{

		dataRoot = root["Shape"];
		ModelName = dataRoot.asString();
		dataRoot = root["Rotated"];
		isRotated = dataRoot.asBool();
		dataRoot = root["MaterialID"];
		materialID = dataRoot.asInt();

		Particles* pPt = static_cast<Particles*>(tshape);
		dataRoot = root["SpawnTime"];
		pPt->mSpawnTime = dataRoot.asFloat();
		dataRoot = root["LifTime"];
		pPt->mLifeTime.x = dataRoot[0].asFloat();
		pPt->mLifeTime.y = dataRoot[1].asFloat();
		dataRoot = root["SpawnNumber"];
		pPt->mSpawnNumber = dataRoot.asInt64();
		dataRoot = root["MinVelocity"];
		pPt->mMinVelocity.x = dataRoot[0].asFloat();
		pPt->mMinVelocity.y = dataRoot[1].asFloat();
		pPt->mMinVelocity.z = dataRoot[2].asFloat();
		dataRoot = root["MaxVelocity"];
		pPt->mMaxVelocity.x = dataRoot[0].asFloat();
		pPt->mMaxVelocity.y = dataRoot[1].asFloat();
		pPt->mMaxVelocity.z = dataRoot[2].asFloat();
		dataRoot = root["MinOffset"];
		pPt->mMinOffset.x = dataRoot[0].asFloat();
		pPt->mMinOffset.y = dataRoot[1].asFloat();
		pPt->mMinOffset.z = dataRoot[2].asFloat();
		dataRoot = root["MaxOffset"];
		pPt->mMaxOffset.x = dataRoot[0].asFloat();
		pPt->mMaxOffset.y = dataRoot[1].asFloat();
		pPt->mMaxOffset.z = dataRoot[2].asFloat();
	}


	if (isRotated)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationX = 90;
		(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 180;
		if(ModelName=="Wall_Wood.obj")
			(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 90;

	}
	
	if (ShapeName == "Particles")
	{
		obj = new DXObject(tshape, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext(), true);
	}
	else
	{
		if (obj) delete obj;
		obj = new DXObject(tshape, GraphicsManager::GetInstance()->GetDirectXHandler()->GetDevice(), GraphicsManager::GetInstance()->GetDirectXHandler()->GetDeviceContext());
	}
	
	obj->SetMaterialID(materialID);

	if(tex)
		obj->SetTexture(DIFFUSE_TEXTURE, tex);
	if(texN)
		obj->SetTexture(NORMAL_TEXTURE, texN);

	this->key = key;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pAs = this;
}

void Asset::DeSerialize(Json::Value& root)
{
	root["Asset"] = (Json::objectValue);
	root["Asset"]["Shape"] = ShapeName;
	if (ShapeName == "Obj" || ShapeName == "FBX")
	{
		root["Asset"]["Model"] = ModelName;
		root["Asset"]["Rotated"] = isRotated;
		root["Asset"]["MaterialID"] = materialID;
	}
	if(TextureName != "")
	root["Asset"]["Texture"] = TextureName;


}

void Asset::HandleEvent(Event* EventToHandle)
{

}