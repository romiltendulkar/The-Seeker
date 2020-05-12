/*------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name:    GameObject

Purpose:
Abstract class for a game object
Language:   C++

Platform:

Compiler:-  Visual Studio 2017 - Visual C++ 14.1


Hardware Requirements:-		Display monitor, keyboard.

Operating System:-		Microsoft Windows 8.1+


Author: Pratyush Gawai

Student login: pratyush.gawai

Student ID: 60000818

Creation date: 10/1/2019

(DEPRECATED)
---------------------------------------------------------*/

#include "pch.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Asset.h"
#include "Components/Body.h"
#include "Components/Controller.h"
#include "Components/Animation.h"
#include "..//Manager/MemoryManager.h"
//include transform, sprite, body

GameObject::GameObject()
{
	for (auto z : LComponent)
		z = nullptr;
}

GameObject::~GameObject()
{
	for (auto z : LComponent)
		delete z;
	LComponent.clear();
}

//Component * GameObject::addComponent(unsigned int type)
//{
//	Component *mpNewComponent = nullptr;
//	switch (type)
//	{
//	case TRANSFORM:
//		mpNewComponent = new Transform();
//		break;
//	case ASSET:
//		mpNewComponent = new Asset();
//		break;
//	/*case BODY:
//		mpNewComponent = new Body();
//		break;
//	case CONTROLLER:
//		mpNewComponent = new Controller();
//		break;*/
//	}
//
//	if (mpNewComponent != nullptr)
//	{
//		LComponent.push_back(mpNewComponent);
//		mpNewComponent->mpGObj = this;
//	}
//	return mpNewComponent;
//}

Component * GameObject::getComponent(unsigned int type)
{
	for (auto z : LComponent)
	{
		if (z->GetType() == type)
			return z;
	}
	return nullptr;
}

void GameObject::Update(float deltaTime)
{
	
}

void GameObject::HandleEvent(Event *HEvent)
{
	for (auto c : LComponent)
	{
		c->HandleEvent(HEvent);
	}
}