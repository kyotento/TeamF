#include "stdafx.h"
#include "Block.h"

Block::Block(){
}

Block::~Block(){}

void Block::SetPos( int x, int y, int z ){
	constexpr float half = WIDTH * 0.5f;

	CVector3 pos{ x * WIDTH + half, y * WIDTH + half, z * WIDTH + half };

	m_model.SetPos( pos );

	if( m_collision ){
		pos.y += half;
		m_collision->SetPosition( pos );
	}
}

void Block::EnableCollision(){
	if( !m_collision ){
		m_collision = std::make_unique<SuicideObj::CCollisionObj>();
		m_collision->SetIsStaticObject( true );
		m_collision->CreateBox( CVector3::Zero(), CQuaternion::Identity(), CVector3::One() * Block::WIDTH );
		m_collision->SetTimer( enNoTimer );

		CVector3 pos = m_model.GetPos();
		pos.y += WIDTH * 0.5f;

		m_collision->SetPosition( pos );
	}
}
