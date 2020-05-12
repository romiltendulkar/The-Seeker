#pragma once
#include "Entity.h"
#include "..\..\Manager\MemoryManager.h"
class Body;
class Transform;
class Tree :public Entity
{
public:
	Tree();
	~Tree();
	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	void HandleEvent(Event* pEventToHandle);
	Body* pBd;
	Transform* pTr;
	unsigned int key;
};

