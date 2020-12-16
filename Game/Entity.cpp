#include "stdafx.h"
#include "Entity.h"
#include "World.h"

Entity::Entity(World* world, EnEntity enEntity) : m_world(world), m_enEntity(enEntity)
{
	world->AddEntity(this);
}

Entity::~Entity()
{
	m_world->RemoveEntity(this);
}
