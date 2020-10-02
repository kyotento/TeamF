#include "stdafx.h"
#include "Player.h"
#include <assert.h>
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h>

namespace {
	float characonRadius = 50.f;			//�L�����R���̔��a�B
	float characonHeight = 100.f;			//�L�����R���̍����B

	float moveSpeed = 20.f;					//�v���C���[�̈ړ����x�B
	float mouseSensitivity = 1.0f;			//�}�E�X���x�B
	float degry;							//�}�E�X�̈ړ��ʂ����Z�������́B
	float radian;

	CVector2 mouseCursorMovePow = CVector2::Zero();		//�}�E�X�J�[�\���̈ړ��ʁB

	CVector3 movePower;						//�L�����R���̈ړ��ɂ�����́B
	CVector3 moveSp;

	CQuaternion rotMousePower;				//�}�E�X�̉�]�ʁB
}

Player::Player()
{
}


Player::~Player()
{
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//�v���C���[���f���̏������B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.cmo");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);

	m_skn = NewGO<GameObj::CSkinModelRender>();
	m_skn->Init(L"Resource/modelData/flore_kari.cmo");
	m_skn->SetPos({ 0.f,0.f,0.f });

	m_charaPos = m_position;
	//�L�����R���̏������B
	m_characon.Init(characonRadius, characonHeight, m_charaPos);

	return true;
}

void Player::Update()
{
	Move();			//�ړ������B
	Rotation();		//��]�����B
}

//�ړ������B
void Player::Move()
{
	//�L�[�{�[�h�̈ړ������B
	if (GetKeyInput('A')) {
		movePower.x -= moveSpeed;
	}
	if (GetKeyInput('D')) {
		movePower.x += moveSpeed;
	}
	if (GetKeyInput('W')) {
		movePower.z += moveSpeed;
	}
	if (GetKeyInput('S')) {
		movePower.z -= moveSpeed;
	}

	movePower.Normalize();

	radian = M_PI / 180 * degry;

	moveSp.z = -sin(radian) * movePower.x * moveSpeed;
	moveSp.x = cos(radian) * movePower.x * moveSpeed;
	//�X�e�B�b�N�̏㉺���͂̏���
	moveSp.z += cos(radian) * movePower.z * moveSpeed;
	moveSp.x += sin(radian) * movePower.z * moveSpeed;
	moveSp.y = 0.0f;

	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSp, 1.f);

	m_skinModelRender->SetPos(m_position);		//�v���C���[�̍��W���w�肷��B

	movePower = CVector3::Zero();				//�L�����R���̈ړ��ʂ��������B

}

//��]�����B
void Player::Rotation()
{
	mouseCursorMovePow = MouseCursor().GetMouseMove();			//�}�E�X�̈ړ��ʂ��擾�B

	degry += mouseCursorMovePow.x * mouseSensitivity;			//�}�E�X�̌����B

	rotMousePower.SetRotationDeg(CVector3::AxisY(), mouseCursorMovePow.x * mouseSensitivity);		//�ړ��ʂ����Ƃ�Y����]�̗͂��v�Z�B

	m_rot.Multiply(rotMousePower);			//����Rotation�ɉ��Z�B

	m_skinModelRender->SetRot(m_rot);

}