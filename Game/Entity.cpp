#include "stdafx.h"
#include "Entity.h"
#include "World.h"

Entity::Entity(World* world) : m_world(world)
{
	world->AddEntity(this);
}

Entity::~Entity()
{
	m_world->RemoveEntity(this);
}
