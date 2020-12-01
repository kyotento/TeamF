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

//自分に当たった時を除くコールバック。
struct ExceptMeClosestConvex : public btCollisionWorld::ClosestConvexResultCallback{
	SuicideObj::CCollisionObj& me;

	//コンストラクタ
	ExceptMeClosestConvex( SuicideObj::CCollisionObj& me, const btVector3& start, const btVector3& end )
		: ClosestConvexResultCallback( start, end ), me( me ){};

	//判定するかどうか判定する
	bool needsCollision( btBroadphaseProxy* proxy0 ) const override{
		//じぶんでない
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

	//初期位置
	const btVector3 start = m_pos;
	btTransform startTra = obj.getWorldTransform();

	//目標位置
	const btVector3 end = m_pos + moveVec;
	btTransform endTra = startTra;
	endTra.setOrigin( end );

	//一番最初に当たった場所の情報を返すコールバック。
	//自分に当たったときを除く。
	ExceptMeClosestConvex callback( m_collision, start, end );

	GetEngine().GetPhysicsWorld().ConvexSweepTest( static_cast<btConvexShape*>( obj.getCollisionShape() ),
												   startTra, endTra, callback );

	//当たったところまで進める。HitFractionは、指定した移動距離を1としたときの、実移動距離を返す。
	m_pos = m_pos + moveVec * callback.m_closestHitFraction;

	m_collision.SetPosition( m_pos );

	//何かに当たったなら問答無用でスピードを０にする。
	if( callback.hasHit() ){
		moveSpeed = CVector3::Zero();
	}

	return m_pos;
}
