/**
 * @file PuzzleManager.h
 * @authors Pratyush Gawai
 * @date 9/21/2019
 * @brief This file has methods to update gameobjects.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "PuzzleManager.h"
#include "MemoryManager.h"
#include "../Source/Entity/PuzzleTile.h"
#include "../Source/Entity/Pillar.h"
#include "ObjectFactoryManager.h"
#include "../Source/Entity/Entity.h"

PuzzleManager::PuzzleManager()
{
}

PuzzleManager::~PuzzleManager()
{
	
}

PuzzleManager * PuzzleManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new PuzzleManager();
	}
	return mInstance;
}

void PuzzleManager::Initialize()
{
	int size = ObjectFactoryManager::GetInstance()->LPTPositions.size();
	for (int i = 0; i < size; ++i)
	{
		int key = ObjectFactoryManager::GetInstance()->LPTPositions.at(i);

		PuzzleTile* pl = static_cast<PuzzleTile*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		pl->color = random(0, 2);

		cout << pl->color << endl;

		pl->SetColor(pl->color);
	}
	for (int i = 0; i < 3; ++i)
	{
		int key = ObjectFactoryManager::GetInstance()->LPillarPositions[i];
		Pillar* pl = static_cast<Pillar*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);

		pl->ResetColor();


	}
	CreateKeyColors();

	for (int i = 0; i < 3; ++i)
	{
		int key = ObjectFactoryManager::GetInstance()->LPillarPositions[i];
		Pillar* pl = static_cast<Pillar*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);

		pl->color = colorkey[i];


	}


	for (int i = 0; i < 3; ++i)
	{
		int key = ObjectFactoryManager::GetInstance()->LPillarPositions[i];
		Pillar* pl = static_cast<Pillar*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);

		pl->color = colorkey[i];
		
	
	}

}



void PuzzleManager::Destroy()
{
	delete mInstance;
}

void PuzzleManager::Update(float rdeltaTime)
{
}



int PuzzleManager::random(int min, int max) //range : [min, max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void PuzzleManager::CreateKeyColors()
{
	colorkey.clear();
	int ch = random(1, 6);
	switch (ch)
	{
	case 1:
	{
		colorkey.emplace_back(0);
		colorkey.emplace_back(1);
		colorkey.emplace_back(2);
		break;
	}
	case 2:
	{
		colorkey.emplace_back(0);
		colorkey.emplace_back(2);
		colorkey.emplace_back(1);
		break;
	}
	case 3:
	{
		colorkey.emplace_back(1);
		colorkey.emplace_back(2);
		colorkey.emplace_back(0);
		break;
	}
	case 4:
	{
		colorkey.emplace_back(1);
		colorkey.emplace_back(0);
		colorkey.emplace_back(2);
		break;
	}
	case 5:
	{
		colorkey.emplace_back(2);
		colorkey.emplace_back(1);
		colorkey.emplace_back(0);
		break;
	}
	case 6:
	{
		colorkey.emplace_back(2);
		colorkey.emplace_back(0);
		colorkey.emplace_back(1);
		break;
	}

	}
}




