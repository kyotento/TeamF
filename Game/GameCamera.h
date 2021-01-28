#pragma once
#include "Block.h"
#include "CameraCollisionSolver.h"
class Player;
//�J�����̃��[�h
enum EnCameraMode {
	EnMode_FPS,   //FPS
	EnMode_TPS,   //TPS
	EnMode_ReverseTPS,    //���ʂ���̃J����
	EnMode_Num
};
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}
	bool Start() override;
	void Update() override;

	//�J�����̃��[�h���擾
	EnCameraMode GetCameraMode()
	{
		return m_mode;
	}

	//���W���擾�B
	CVector3 GetPos() const
	{
		return m_position;
	}

	/// <summary>
	/// ���[������]�p�x��ݒ�
	/// </summary>
	/// <param name="deg">�p�x</param>
	/// <param name="isLock">���b�N���邩</param>
	void SetRollDeg(float deg, bool isLock = false) {
		m_rollDeg = deg;
		m_isRollLock = isLock;
	}

private:
	//TPS
	void TPS();
	//FPS
	void FPS();
	//���ʂ���̃J����
	void ReverseTPS();
public:
	Player* m_player = nullptr;							//�v���C���[
	GameObj::PerspectiveCamera* m_camera;				//�Q�[���J����
	CVector3 m_position = CVector3::One() * 10.0f;		//���W
	CVector3 m_target = CVector3::Zero();				//�����_
	const float m_radius = 4.f* Block::WIDTH;			//�����_�ƍ��W�̋���
	float m_radianY = 0.0f;								//Y���̉�](���W�A��)
	float m_radianXZ = 0.0f;							//XZ���̉�](���W�A��)
	EnCameraMode m_mode = EnMode_FPS;					//�J�����̃��[�h
	float m_viewAngleDeg = 90.0f;						//����p
	float m_rollDeg = -0.5f;								//�J�����̉�]�p�x(���[����)
	bool m_isRollLock = false;							//���[������]�����b�N���邩
	CCameraCollisionSolver m_cameraCollisionSolver;
};

