#include "stdafx.h"
#include "SimpleMoveObj.h"

void SimpleMoveObj::CreateBox( const CVector3& pos, const CQuaternion& rot, const CVector3& size ){
	m_collision.CreateBox( pos, rot, size );
	SetPos( pos ); SetRot( rot );
}
void SimpleMoveObj::CreateCapsule( const CVector3& pos, const CQuaternion& rot, float radius, float height ){
	m_collision.CreateCapsule( pos, rot, radius, height );
	SetPos( pos ); SetRot( rot );
}

void SimpleMoveObj::CreateSphere( const CVector3& pos, const CQuaternion& rot, float radius ){
	m_collision.CreateSphere( pos, rot, radius );
	SetPos( pos ); SetRot( rot );
}

//�����ɓ����������������R�[���o�b�N�B
struct ExceptMeClosestConvex : public btCollisionWorld::ClosestConvexResultCallback{
	SuicideObj::CCollisionObj& me;

	//�R���X�g���N�^
	ExceptMeClosestConvex( SuicideObj::CCollisionObj& me, const btVector3& start, const btVector3& end )
		: ClosestConvexResultCallback( start, end ), me( me ){};

	//���肷�邩�ǂ������肷��
	bool needsCollision( btBroadphaseProxy* proxy0 ) const override{
		//���Ԃ�łȂ�
		if( (btCollisionObject*)proxy0->m_clientObject == ( btCollisionObject* )&( me.GetCollisionObject() ) ){
			return false;
		}
		return ClosestConvexResultCallback::needsCollision( proxy0 );
	};
};

const CVector3 & SimpleMoveObj::Execute( CVector3 & moveSpeed, float deltaTime ){

	const float moveLenSqr = moveSpeed.x*moveSpeed.x + moveSpeed.y*moveSpeed.y + moveSpeed.z*moveSpeed.z;
	if( moveLenSqr < 0.0000001f ){
		return m_pos;
	}

	const CVector3 moveVec = moveSpeed * deltaTime;

	btGhostObject& obj = m_collision.GetCollisionObject();

	//�����ʒu
	const btVector3 start = m_pos;
	btTransform startTra = obj.getWorldTransform();

	//�ڕW�ʒu
	const btVector3 end = m_pos + moveVec;
	btTransform endTra = startTra;
	endTra.setOrigin( end );

	//��ԍŏ��ɓ��������ꏊ�̏���Ԃ��R�[���o�b�N�B
	//�����ɓ��������Ƃ��������B
	ExceptMeClosestConvex callback( m_collision, start, end );

	GetEngine().GetPhysicsWorld().ConvexSweepTest( static_cast<btConvexShape*>( obj.getCollisionShape() ),
												   startTra, endTra, callback );

	//���������Ƃ���܂Ői�߂�BHitFraction�́A�w�肵���ړ�������1�Ƃ����Ƃ��́A���ړ�������Ԃ��B
	m_pos = m_pos + moveVec * callback.m_closestHitFraction;

	m_collision.SetPosition( m_pos );

	//�����ɓ��������Ȃ�ⓚ���p�ŃX�s�[�h���O�ɂ���B
	if( callback.hasHit() ){
		moveSpeed = CVector3::Zero();
	}

	return m_pos;
}
