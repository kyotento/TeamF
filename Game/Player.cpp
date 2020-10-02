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
	float degry;							//�v���C���[�̌���(�P��::�f�O���[)�B
	float radian;							//�v���C���[�̌���(�P��::���W�A��)�B

	CVector2 mouseCursorMovePow = CVector2::Zero();		//�}�E�X�J�[�\���̈ړ��ʁB

	CVector3 moveDirection;					//�L�����R���̈ړ��ɂ���������B
	CVector3 moveSp;						//�L�����R���̈ړ��ɂ�����́B

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
		moveDirection.x -= 1.f;
	}
	if (GetKeyInput('D')) {
		moveDirection.x += 1.f;
	}
	if (GetKeyInput('W')) {
		moveDirection.z += 1.f;
	}
	if (GetKeyInput('S')) {
		moveDirection.z -= 1.f;
	}

	moveDirection.Normalize();			//���K���B
	m_front = moveDirection;			//�v���C���[�̑O���������擾�B

	radian = M_PI / 180 * degry;	//�v���C���[�̌��������W�A���P�ʂɕύX�E

	//���E���͂̏����B
	moveSp.z = -sin(radian) * moveDirection.x * moveSpeed;
	moveSp.x = cos(radian) * moveDirection.x * moveSpeed;
	//�㉺���͂̏����B
	moveSp.z += cos(radian) * moveDirection.z * moveSpeed;
	moveSp.x += sin(radian) * moveDirection.z * moveSpeed;
	moveSp.y = 0.0f;

	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSp, 1.f);

	m_skinModelRender->SetPos(m_position);		//�v���C���[�̍��W���w�肷��B

	moveDirection = CVector3::Zero();			//�L�����R���̌����ƈړ��ʂ��������B

}

//��]�����B
void Player::Rotation()
{
	//���t���[������������K�v�����邽�߁B
	m_right = CVector3::Right();
	m_front = CVector3::Front();

	mouseCursorMovePow = MouseCursor().GetMouseMove();			//�}�E�X�̈ړ��ʂ��擾�B

	degry += mouseCursorMovePow.x * mouseSensitivity;			//�}�E�X�̌����B

	rotMousePower.SetRotationDeg(CVector3::AxisY(), mouseCursorMovePow.x * mouseSensitivity);		//�ړ��ʂ����Ƃ�Y����]�̗͂��v�Z�B

	m_rot.Multiply(rotMousePower);			//����Rotation�ɉ��Z�B

	m_skinModelRender->SetRot(m_rot);
	
	//�J�����Ŏg���悤�B
	m_rot.Multiply(m_right);
	m_rot.Multiply(m_front);

}