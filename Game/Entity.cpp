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

float Entity::GetLightColorOnPosition()const {
	if (m_world)
	{
		char* light = m_world->GetLightData(IntVector3((GetPos() + CVector3::Up() * Block::WIDTH * 0.5f) / Block::WIDTH));
		char* sky = m_world->GetSkyLightData(IntVector3((GetPos() + CVector3::Up() * Block::WIDTH * 0.5f) / Block::WIDTH));
		if (light && sky) {
			return max(CMath::Square(LightUtil::DRAWING_LIGHT[*light]), CMath::Square(LightUtil::DRAWING_SKY_LIGHT[*sky]) * m_world->GetSun()->GetSkyLightPower());
		}
	}
	return 1.0f;
}

void Entity::LightingSkinModel(SkinModel& model) {
	CVector4 color = GetLightColorOnPosition();
	color.w = 1.0f;
	model.FindMaterialSetting([&color](MaterialSetting* mat) {
		mat->SetAlbedoScale(color);
		mat->SetShininess(0.0f);
		//mat->SetLightingEnable(false);
	});
}