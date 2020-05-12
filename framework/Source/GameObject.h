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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "pch.h"
#include "Events/Event.h"

class Component;
class GameObject
{
public:
	GameObject();
	~GameObject();

	//Component * addComponent(unsigned int type);
	Component * getComponent(unsigned int type);
	void Update(float deltaTime);
	void HandleEvent(Event *HEvent);

public:
	std::vector<Component *> LComponent;
};

#endif