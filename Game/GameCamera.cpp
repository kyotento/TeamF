#include "stdafx.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "Player.h"

bool GameCamera::Start()
{
	m_position = CVector3(00.0f, 20.0f, 10.0f);
	m_camera = NewGO<GameObj::PerspectiveCamera>();
	m_target = CVector3(0.0f, 10.0f, 0.0f);
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
	SetMainCamera(m_camera);
	return true;
}

void GameCamera::Update()
{
	TPS();
}

void GameCamera::TPS()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	CVector3 stickR = CVector3::Zero();
	float degreeSpeed = 60.0f;
	const float maxDegreeXZ = 70.0f;
	const float minDegreeXZ = 5.0f;
	stickR = Pad(0).GetStick(R);	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	stickR.x = -stickR.x;
	stickR.z = 0.0f;
	//�E�X�e�B�b�N�̓���
	//�E�X�e�B�b�N
	m_degreeY += -stickR.x * degreeSpeed / GetEngine().GetStandardFrameRate();
	m_degreeXZ += -stickR.y * degreeSpeed / GetEngine().GetStandardFrameRate();

	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//�p�x�����W�A���P�ʂɒ���
	float radianY = M_PI / 180 * m_degreeY;
	float radianXZ = M_PI / 180 * m_degreeXZ;

	m_target = m_player->GetPosition() + m_player->GetRight()* 3.0f;
	m_target.y += 4.0f;
	//Y������ɉ�]������B
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), radianY);
	CVector3 toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(toPos);
	//�㉺�̉�]�B
	//�܂��͉񂷎����v�Z����B
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	//�x�N�g���𐳋K������B
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, radianXZ);
	qRot.Multiply(toPos);
	toPos *= m_radius;
	m_position = m_target + toPos;
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
}