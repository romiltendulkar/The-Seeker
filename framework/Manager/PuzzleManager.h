/**
 * @file PuzzleManager.h
 * @authors Pratyush Gawai
 * @date 9/21/2019
 * @brief This file has methods to update gameobjects.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#pragma once
#include "../Source/pch.h"



class PuzzleManager
{
public:
	PuzzleManager();

	/**
	clear all game object
	*/
	~PuzzleManager();

	/**
	This function return Manager itself
	*/
	static PuzzleManager* GetInstance();

	/**
	delete manager
	*/
	void Destroy();

	/**
	upadate all entity
	*/
	void Update(float rdeltaTime);
	
	void Initialize();
	
	int random(int min, int max);
	int r, g, b;
	
	void CreateKeyColors();
	
	int correct = 0;

	std::vector<int> colorkey;

private:
	static PuzzleManager* mInstance;
	
};

