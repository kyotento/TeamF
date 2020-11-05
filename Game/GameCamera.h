#pragma once
#include "Block.h"
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
	const float m_radius = 4.f* Block::WIDTH;						//�����_�ƍ��W�̋���
	float m_radianY = 0.0f;								//Y���̉�](���W�A��)
	float m_radianXZ = 0.0f;							//XZ���̉�](���W�A��)
	EnCameraMode m_mode = EnMode_FPS;					//�J�����̃��[�h
};

