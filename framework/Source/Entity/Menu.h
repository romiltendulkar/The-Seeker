#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;
class Asset;

class Menu : public Entity
{
public:
	Menu();
	virtual ~Menu();

	void Serialize(Json::Value root, unsigned int position);

	void Update(float deltaTime);

public:
	unsigned int key;//entity position in the pool
	int ownerEnemy;
	Body* pBd;
	Animation* pAn;
	Asset* pAs;
	std::string hstring;
};

