//! @file
#pragma once

//! @brief �����P���ȓ������������铖���蔻��N���X�B
//! @details �ǂɓ����������ɁA����悤�ɓ����������s�킸�A�P���ɂ߂荞�ݕ����ɖ߂��������s���L�����R���ƍl���Ă��炦�΂����B
//! @details ConvexSweepTest��MeshCollider���Ή����ĂȂ����߁AMeshCollider�͎g���Ȃ��B
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
	* @brief	���s�B
	* @param[in, out]	moveSpeed	�ړ����x�B�����ɓ����������ɂ̓[���x�N�g���ɂ��܂��B
	* @param[in]	deltaTime	�o�ߎ��ԁB�P�ʂ͕b�B�f�t�H���g�ł́A�P�t���[���̌o�ߎ��Ԃ��n����Ă��܂��B
	* @return	�ړ���̍��W�B
	*/
	const CVector3& Execute( CVector3& moveSpeed, float deltaTime = 1.0f / GetStandardFrameRate() );

private:
	CVector3 m_pos;
	CQuaternion m_rot;

	SuicideObj::CCollisionObj m_collision;
};