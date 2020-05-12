#include "../pch.h"
#include "Entity.h"


Entity::Entity(ENTITY_TYPE TYPE):mEntityType(TYPE)
{
	complete = false;
}

Entity::~Entity()
{
}
