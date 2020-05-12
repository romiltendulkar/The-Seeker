/**
 * @file LightDetail.h
 * @authors  Pratyush Gawai
 * @date 10/1/19
 * @brief This file gives details about the light.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef LIGHTDETAIL_H
#define LIGHTDETAIL_H

#include "../pch.h"
#include "Component.h"
class Light;

class LightDetail : public Component
{
public:
	LightDetail();
	~LightDetail();

	//void Serialize(JSONObject root);
	void Update(float deltaTime);

	/**
	Serializes LightDetail
	Root : JSON Root
	key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int key);

	/**
	DeSerialize LightDetail
	*/
	void DeSerialize(Json::Value& root);

	void HandleEvent(Event *EventToHandle);

public:


	unsigned int key;
	Light* pLt;
};

#endif
