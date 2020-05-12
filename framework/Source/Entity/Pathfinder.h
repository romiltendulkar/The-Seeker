#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"



struct res_pair
{
	int x, y;
};

class Controller;
class Animation;
class Body;
class Transform;
class Pathfinder : public Entity
{
public:
	/**
   In this constructor we initialize the variables.
	*/
	Pathfinder();
	virtual ~Pathfinder();
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
	/**
	   In this function, UI for the player, health bar or time bar is created.
	*/
	void HandleEvent(Event* pEvent);
public:
	unsigned int key;//entity position in the pool
	bool path_calc = false;
	unsigned int hPos;
	Controller* pCr;
	Animation* pAn;
	Body* pBd;
	Transform* pTr;
	float floor_x, floor_y;//left bottom  coordinates.
	float floor_scale_x, floor_scale_y;
	grid** map;
	float start_x, start_y, end_x = 0, end_y = 0, move_x, move_y;
	std::vector<unsigned int> LWallPositions;
	std::vector<unsigned int> LLightPositions;
	std::vector<unsigned int> LBoxPositions;
	std::vector<res_pair> points;
	std::set <std::pair<float, std::pair<int, int>>> openlist;
	std::pair<float, std::pair<int, int>>n1;
	std::pair<float, std::pair<int, int>>n;
	int index_offset = 1;
	int player_x, player_y;
	int oplayer_x, oplayer_y;
	bool player_moved = true;
	bool period_check = false;
	static int frame_counter;
	float Octile(float start_x, float start_y, float goal_x, float goal_y);
	node** proxy_map;
	unsigned int guide_key;
	int updowncounter = 30;
};


