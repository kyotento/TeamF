//! @file
#pragma once

//! @brief ごく単純な動きをさせられる当たり判定クラス。
//! @details 壁に当たった時に、滑るように動く処理を行わず、単純にめり込み分後ろに戻す処理を行うキャラコンと考えてもらえばいい。
//! @details ConvexSweepTestにMeshColliderが対応してないため、MeshColliderは使えない。
//! @author Takayama
class SimpleMoveObj{
public:

	void CreateBox( const CVector3& pos, const CQuaternion& rot, const CVector3& size );

	void CreateCapsule( const CVector3& pos, const CQuaternion& rot, float radius, float height );

	void CreateSphere( const CVector3& pos, const CQuaternion& rot, float radius );

	void SetPos( const CVector3& pos ){
		m_pos = pos;
	}

	CVector3 GetPos() const{
		return m_pos;
	}

	void SetRot( const CQuaternion& rot ){
		m_rot = rot;
		btQuaternion btRot( rot.x, rot.y, rot.z, rot.w );
		m_collision.GetCollisionObject().getWorldTransform().setRotation( btRot );
	}

	CQuaternion GetRot() const{
		return m_rot;
	}

	SuicideObj::CCollisionObj& GetCollision(){
		return m_collision;
	}

	/*!
	* @brief	実行。
	* @param[in, out]	moveSpeed	移動速度。何かに当たった時にはゼロベクトルにします。
	* @param[in]	deltaTime	経過時間。単位は秒。デフォルトでは、１フレームの経過時間が渡されています。
	* @return	移動後の座標。
	*/
	const CVector3& Execute( CVector3& moveSpeed, float deltaTime = 1.0f / GetStandardFrameRate() );

private:
	CVector3 m_pos;
	CQuaternion m_rot;

	SuicideObj::CCollisionObj m_collision;
};