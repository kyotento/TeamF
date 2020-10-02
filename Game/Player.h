#pragma once
#include "../physics/character/CCharacterController.h"
class Player : public IGameObject
{
public:
	Player();
	~Player();

	bool Start();
	void Update();

	/// <summary>
	/// �v���C���[�̈ړ������B
	/// </summary>
	void Move();

	/// <summary>
	/// �v���C���[�̉�]�����B
	/// </summary>
	void Rotation();

	/// <summary>
	/// �v���C���[�̑O���������擾�B
	/// </summary>
	/// <returns>�v���C���[�̑O������</returns>
	CVector3 GetFront()
	{
		return m_front;
	}

	/// <summary>
	/// �v���C���[�̉E�������擾����B
	/// </summary>
	/// <returns>�v���C���[�̉E����</returns>
	CVector3 GetRight()
	{
		return m_right;
	}

	/// <summary>
	/// ���W����肷��B
	/// </summary>
	/// <returns>�v���C���[�̍��W</returns>
	CVector3 GetPosition()
	{
		return m_position;
	}


private:

	CVector3 m_position = CVector3::Zero();				//�v���C���[�̍��W�B
	CVector3 m_charaPos = CVector3::Zero();				//�L�����R���̍��W�B
	CVector3 m_scale = CVector3::One();					//�v���C���[���f���̃X�P�[���B
	CVector3 m_front = CVector3::Front();				//�v���C���[�̑O�������B
	CVector3 m_right = CVector3::Right();				//�v���C���[�̉E�����B

	CQuaternion m_rot = CQuaternion::Identity();		//�v���C���[�̉�]�B

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�v���C���[�̃��f���B
	CCharacterController m_characon;					//�L�����R���B


private://���B

	GameObj::CSkinModelRender* m_skn = nullptr;			//�����B


 };

