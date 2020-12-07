#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"

namespace {
	const float m_pulsPosY = 75.0f;				//Y�͕ʂő����Ă��炵�܂��B
	const float m_playerPulsPosX = 1.0f;		//X�̓v���C���[�̂ق��ɑ����܂��B
	const float m_pulsPosZ = 50.0f;			//������������ɂ��Ă܂��B
	const float m_mullPos = 50.0f;				//�ЂƂ܂��O�x�N�g���̐��l��傫������ϐ��B
	float m_degreeY = 0.0f;
	const float maxDegreeY = 70.0f;				//Y���̉�]�̍ő�l�B
	const float minDegreeY = -50.0f;			//Y���̉�]�̍ŏ��l�B

	CVector3 neerPosition = CVector3::Zero();	//��pos�B
}

ItemDisplay::ItemDisplay()
{
}

ItemDisplay::~ItemDisplay()
{
	DeleteGO(m_skinModelRender);
}

bool ItemDisplay::Start()
{
	//���f������
	InitModel();
	return true;
}

void ItemDisplay::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	//�Ǐ]�����B
	Follow();

	//��]
	Rotation();

	//�؂�ւ��̏����B
	Switching();
}

//���f�������B
void ItemDisplay::InitModel()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
}

//���f���̒Ǐ]�B
void ItemDisplay::Follow()
{
	//�}�E�X�̈ړ��ʂ��擾�B
	const float turnMult = 20.0f;
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetDeltaTimeSec();
	//��]����
	m_degreeY += mouseCursorMovePow.x;
	//XZ���̉�]�𐧌��B
	if (m_degreeY < minDegreeY) {
		m_degreeY = minDegreeY;
	}
	else if (m_degreeY > maxDegreeY) {
		m_degreeY = maxDegreeY;
	}
	//m_forward = m_gameCamera->GetPos();
	//auto m_rot = CMatrix::Identity();
	//m_rot.MakeRotationFromQuaternion(m_rotation);
	//m_forward.x = m_rot.m[2][0];
	//m_forward.y = m_rot.m[2][1];
	//m_forward.z = m_rot.m[2][2];

	//�݂��O���ɒu�������B
	m_forward = m_player->GetFront();
	m_forward *= m_mullPos;
	m_forward.y += m_pulsPosY;
	m_forward.z += m_pulsPosZ;
	//�����Ă܂��B
	m_position = m_position + m_forward;
	//���W�����f���ցB
	m_skinModelRender->SetPos(m_position);
}

//�؂�ւ��i�܂����g���Ȃ��ł��B�j
void ItemDisplay::Switching()
{

}

//�r�̉�]����
void ItemDisplay::Rotation()
{
	//���f������]�������B
	m_rotation = m_player->GetRot();
	//m_rotation.SetRotationDeg(CVector3::AxisY(),45.0f);
	//���f���ցB
	m_skinModelRender->SetRot(m_rotation);
}