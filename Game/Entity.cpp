#include "stdafx.h"
#include "Entity.h"
#include "World.h"

Entity::Entity(EnEntity enEntity) : m_enEntity(enEntity)
{}

Entity::~Entity()
{
	if( m_world ){
		m_world->RemoveEntity( this );
	}
}
