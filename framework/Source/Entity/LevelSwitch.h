#pragma once
#include "Entity.h"
#include "..\..\Manager\MemoryManager.h"
class Body;
class Transform;
class LevelSwitch :public Entity
{
public:
	LevelSwitch();
	void Serialize(Json::Value root, unsigned int position);
	~LevelSwitch();
	void Update(float deltaTime);
	void HandleEvent(Event* pEventToHandle);
	Body* pBd;
	Transform* pTr;
	unsigned int key;
	string mLevelName;
};

