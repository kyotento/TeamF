#include "stdafx.h"
#include "Entity.h"
#include "World.h"

Entity::Entity(EnEntity enEntity, bool useBulletColision) : m_enEntity(enEntity), m_useBulletColision(useBulletColision)
{}

Entity::~Entity()
{
	if( m_world ){
		m_world->RemoveEntity( this );
	}
}
