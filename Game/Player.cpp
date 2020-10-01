#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 

bool Player::Start()
{
	m_model.Init(L"Resource/modelData/GrassBlock.cmo");
	m_model.SetScale(CVector3::One() * 0.01f);
	m_gameCamera->SetDegreeY(m_degree);
	return true;
}

void Player::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	Move();
	Turn();
}

void Player::Move()
{
	const float mult = 4.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾
	CVector3 stickL;
	if (m_gameCamera->GetCameraMode() == EnMode_ReverseTPS) {
		stickL.y = Pad(0).GetStick(L).y;
		stickL.x = -Pad(0).GetStick(L).x;	//�A�i���O�X�e�B�b�N�̓��͗ʂ��擾�B
	}
	else {
		stickL.y = -Pad(0).GetStick(L).y;
		stickL.x = Pad(0).GetStick(L).x;	//�A�i���O�X�e�B�b�N�̓��͗ʂ��擾�B
	}

	CVector3 moveSpeed = CVector3::Zero();

	/*if (m_gameCamera->GetIsFPS()) {
		stickL.y = Pad(0).GetStick(L).y;
		stickL.x = -Pad(0).GetStick(L).x;	//�A�i���O�X�e�B�b�N�̓��͗ʂ��擾�B
	}
	else {
		stickL.y = -Pad(0).GetStick(L).y;
		stickL.x = Pad(0).GetStick(L).x;	//�A�i���O�X�e�B�b�N�̓��͗ʂ��擾�B
	}*/
	moveSpeed.z = sin(m_radian)*stickL.x * mult;
	moveSpeed.x = -cos(m_radian)*stickL.x * mult;
	//�X�e�B�b�N�̏㉺���͂̏���
	moveSpeed.z += cos(m_radian)*stickL.y * mult;
	moveSpeed.x += sin(m_radian)*stickL.y * mult;
	moveSpeed.y = 0.0f;
	m_position += moveSpeed * mult / GetEngine().GetStandardFrameRate();
	m_model.SetPos(m_position);
}

void Player::Turn()
{
	const float Mult = 60.0f;

	CVector3 rotation = { 0.0f,0.0f,0.0f };
	//���@�̊p�x�̍���
	m_radian = M_PI / 180 * m_degree;
	//�E�X�e�B�b�N
	CVector2 stickR;
	stickR.x = -Pad(0).GetStick(R).x;	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	//����
	//�E�X�e�B�b�N
 	float sDegree = -stickR.x * Mult / GetEngine().GetStandardFrameRate();
	//��]����
	m_degree += sDegree;
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degree);

	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);

	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}