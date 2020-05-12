#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;
class Transform;

class WEnemy : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	WEnemy();
	virtual ~WEnemy();
	/**
	Serializes asset
	Root : JSON Root
	key : position in objectpool
	*/
	void Serialize(Json::Value root, unsigned int position);
	/**
	   In this function, every component of the said entity is updated.

	  \param rdeltaTime | Delta time between frames.
	*/
	void Update(float deltaTime);
	void CreateViewCone();
	void CreateSpotLight();
	void SetViewBoxDim(int key,int dimx,int dimy);
	void SetViewBoxPos(int key,int i); //set position near to enemy pos every frame
public:
	unsigned int key;//entity position in the pool
	
	int spos;
	bool mGoingUp = true;
	Body* pBd;
	Animation* pAn;
	Transform* pTr;
	float mTimer;
	float mTimerLimit;
	vector<int> LVPos;
	vector<int> offset;

	bool can_see_player = false;
	int wall_at = -1;
	bool colliding_player = false;
	int colliding_player_position = -1;
	bool colliding_wall = false;
	int colliding_wall_position = -1;
};
