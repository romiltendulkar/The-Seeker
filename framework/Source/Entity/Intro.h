/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Intro.h
Purpose:
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 04/13/2019
- End Header --------------------------------------------------------*/

#pragma once


#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;
class Asset;

class Intro : public Entity
{
public:
	Intro();
	virtual ~Intro();

	void Serialize(Json::Value root, unsigned int position);

	void Update(float deltaTime);

public:
	unsigned int key;//entity position in the pool
	Body* pBd;
	Animation* pAn;
	Asset* pAs;
	vector<DXTexture*> texturelist;
	std::string hstring;
	float mTime = 3.0f;
	int index = 0;
	bool stop;
};