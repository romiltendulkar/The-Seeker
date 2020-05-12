#pragma once

#include "../pch.h"
#include "../../Dependencies/jsoncpp/json/json.h"
#include "..\Events\Event.h"
//#include "../../Manager/MemoryManager.h"

enum class ENTITY_TYPE
{
	LP,
	BOSS2,
	STOPPER,
	MOVINGPLATFORM,
	DOOR,
	CONDITIONBOX,
	RP,
	VISIBLE_BOX,
	PLAYER,
	BULLET,
	PLATFORM,
	PATHFINDER,
	GUIDEBOX,
	BLOCK,
	FLOOR,
	COLLIDER,
	BOSS1,
	EBULLET,
	ENEMY,
	RIGHT,
	LEFT,
	HEALTHBAR,
	WENEMY,
	VIEWBOX,
	SPOTBOX,
	SPOT_LIGHT,
	DIRECT_LIGHT,
	POINT_LIGHT,
	SEEKER,
	SEEKERBOX,
	TIMEBAR,
	UIELEMENT,
	PUZZLETILE,
	SHOOTER,
	CHARGER,
	PILLAR,
	SWITCH,
	HINT,
	LOOTDROP,
	EXPLODER,
	GATE,
	PADDING,
	MENU,
	CREDITS,
	INTRO,
	LEVELSWITCH,
	TREE
};

class Entity
{
public:

	Entity(ENTITY_TYPE TYPE);
	virtual ~Entity();
	virtual void Update(float deltaTime) {}
	//ENTITY_TYPE GetEntityType();
	//virtual void Serialize(Json::Value root, unsigned int position) {}
	ENTITY_TYPE GetEntityType()
	{
		return mEntityType;
	}
	virtual void HandleEvent(Event* pEvent) {}
public:
	unsigned int EntityPosition;
	ENTITY_TYPE mEntityType ;
	std::string mEntityName;
	std::string mDataFile;
	bool complete;
	bool runtime;
private:
	

};
