#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"

Block::Block(){
}

Block::~Block(){
	m_raytraceModel.Release();
}

void Block::SetPos( int x, int y, int z ){
	constexpr float half = WIDTH * 0.5f;

	CVector3 pos{ x * WIDTH + half, y * WIDTH, z * WIDTH + half };

	m_model.SetPos( pos );

	if( m_collision ){
		pos.y += half;
		m_collision->SetPosition( pos );
	}
}

void Block::SetPosWithWorldPos(const CVector3& worldpos) {
	m_model.SetPos(worldpos);

	if (m_collision) {
		m_collision->SetPosition(worldpos + CVector3::Up() * (WIDTH * 0.5f));
	}
}

void Block::EnableCollision(){
	if( !m_collision ){
		m_collision = std::make_unique<SuicideObj::CCollisionObj>();
		m_collision->SetIsStaticObject( true );
		m_collision->CreateBox( CVector3::Zero(), CQuaternion::Identity(), CVector3::One() * Block::WIDTH );
		m_collision->SetTimer( enNoTimer );
		m_collision->SetIsHurtCollision(true);	//�������画����Ƃ�Ȃ��B
		m_collision->SetName(L"Block");			//�R���W�����ɖ��O�B
		m_collision->SetPointer(this);			//�|�C���^��ݒ�B
		//m_collision->GetCollisionObject().setUserIndex(enBlock);
		//m_collision->GetCollisionObject().setUserPointer(this);		

		CVector3 pos = m_model.GetPos();
		pos.y += WIDTH * 0.5f;

		m_collision->SetPosition(pos);
	}
}
