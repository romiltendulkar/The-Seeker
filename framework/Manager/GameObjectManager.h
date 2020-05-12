/**
 * @file GameObjectManager.h
 * @authors Pratyush Gawai
 * @date 9/21/2019
 * @brief This file has methods to update gameobjects.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#pragma once
#include "../Source/pch.h"

class GameObject;
class GameState;

class GameObjectManager
{
public:
	GameObjectManager();
	
	/**
	clear all game object
	*/
	~GameObjectManager();

	/**
	This function return Manager itself
	*/
	static GameObjectManager* GetInstance();

	/**
	delete manager
	*/
	void Destroy();

	/**
	upadate all entity
	*/
	void Update(float rdeltaTime);
	std::vector<GameObject *>LGameObject;
	std::vector<std::pair<GameObject*, int>> LIndirection;
	std::vector<GameObject*> LGraveyard;
	
	void DeleteObject(int id);
	void DeleteObject(GameObject*);
	
	void LinkObject(int id,GameObject* go);
private:
	static GameObjectManager* mInstance;
	GameState *pGameState;
};

