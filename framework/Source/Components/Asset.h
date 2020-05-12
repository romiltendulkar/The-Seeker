/**
 * @file Asset.cpp
 * @authors Vineet Amarjeet Dogra, Pratyush Gawai
 * @date 10/1/19
 * @brief This file governs how the asset component is serialized and is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef ASSET_H
#define ASSET_H
#include "../pch.h"
#include "Component.h"
#include "../Graphics/DXObject.h"
#include "../Graphics/DXTexture.h"
class Asset : public Component
{
public :
	Asset();
	~Asset();
	void Update(float deltaTime);

	/**
	Serializes asset
	Root : JSON Root
	key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int key);

	/**
	DeSerialize asset
	*/
	void DeSerialize(Json::Value& root);

	DXObject* obj;
	Shape* tshape;//temporary shape which will hold the actual shape from the file
	DXTexture* tex;
	DXTexture* texN;
	std::string ShapeName, ModelName, TextureName;

	unsigned int key;
	bool isRotated;
	int materialID;

	void HandleEvent(Event *EventToHandle);
	
};
#endif
