#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "ItemData.h"
#include "Chunk.h"
#include "GameMode.h"

namespace {
	const float turnMult = 30.0f;			//�v���C���[�̉�]���x�B
	const float maxDegreeXZ = 70.0f;		//XZ���̉�]�̍ő�l�B
	const float minDegreeXZ = -50.0f;		//XZ���̉�]�̍ŏ��l�B
	const float moveMult = 8.0f;			//�v���C���[�̈ړ����x�B
	const float move = 1.0f;				//�ړ����x(��{�I�ɂ͐G��Ȃ�)�B
	const float gravitationalAcceleration = 0.3f;		//�d�͉����x�B
	CVector3 stickL = CVector3::Zero();		//WSAD�L�[�ɂ��ړ���
	CVector3 moveSpeed = CVector3::Zero();		//�v���C���[�̈ړ����x(��������)�B

}

Player::Player()
{
	//�A�j���[�V�����̐ݒ�B
	m_animationClip[enAnimationClip_Idle].Load(L"Assets/animData/player_idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_move].Load(L"Assets/animData/player_move.tka");
	m_animationClip[enAnimationClip_move].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shift].Load(L"Assets/animData/player_shift.tka");
	m_animationClip[enAnimationClip_shift].SetLoopFlag(true);
	m_animationClip[enAnimationClip_excavate].Load(L"Assets/animData/player_Excavate.tka");
	m_animationClip[enAnimationClip_excavate].SetLoopFlag(true);
}

Player::~Player()
{
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//�v���C���[�N���X�̏������B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Assets/modelData/player.tkm", m_animationClip, enAnimationClip_Num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetRot(m_rotation);

	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_position);

	//�C���x���g���\�̏������B
	for (int i = 0; i < inventryWidth; i++) {
		//m_inventoryList[i] = new Inventory();
		//m_inventoryList[i]->s_item = GetItemData().GetItem(enCube_None);
	}

	return true;
}

void Player::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
	}

	if( GetKeyDown( 'C' ) ){
		static bool lock = true;
		MouseCursor().SetLockMouseCursor( lock = !lock );
	}

	Move();
	//��]�����B
	Turn();
	//�v���C���[�̏�ԊǗ��B0
	StateManagement();
}

//�L�[�{�[�h�̓��͏��Ǘ��B
void Player::KeyBoardInput()
{
	Dash();		//���鏈���B

	//�e�L�[���͂ɂ��ړ��ʂ��v�Z
	//if (!m_doubleCilckFlag) {
	if (GetKeyInput('W')) {
		stickL.y = -move;
	}
	else if (GetKeyInput('S')) {
		stickL.y = move;
	}
	if (GetKeyInput('A')) {
		stickL.x = -move;
	}
	else if (GetKeyInput('D')) {
		stickL.x = move;
	}

	if (m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {		//�N���G�C�e�B�u�̂Ƃ��B
		if (GetKeyInput(VK_SHIFT)) {
			stickL.z = -move;
		}
		else if (GetKeyInput(VK_SPACE)) {
			stickL.z = move;
		}
	}
}

//���鏈���B
void Player::Dash()
{
	if (GetKeyUp('W')) {
		m_doubleCilckFlag = true;
	}
	if (m_doubleCilckFlag) {
		if (m_doubleClickTimer <= 0.2f && m_playerState != enPlayerState_run) {
			m_doubleClickTimer += GetEngine().GetRealDeltaTimeSec();	//�^�C�}�[��i�߂�B
		}
		if (m_doubleClickTimer <= 0.2f) {
			if (GetKeyDown('W')) {		//���点��B
				m_playerState = enPlayerState_run;
				m_runFlag = true;
			}

			if (GetKeyUp('W') && m_runFlag) {
				m_playerState = enPlayerState_idle;		//����̂���߂�B
				m_doubleClickTimer = 0.f;
				m_doubleCilckFlag = false;
				m_runFlag = false;
			}
		}
		else {			//��莞�Ԍo�߂������B
			m_playerState = enPlayerState_idle;
			m_doubleClickTimer = 0.f;
			m_doubleCilckFlag = false;
		}
	}
}

//�ړ������B
void Player::Move()
{
	KeyBoardInput();		//�L�[�{�[�h�̓��͏��B
	stickL.Normalize();

	//���E���͂̏���
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//�㉺���͂̏���
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = stickL.z;

	Jump();   //�W�����v�����B

	moveSpeed.x *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	moveSpeed.y *= moveMult * GetEngine().GetStandardFrameRate();
	moveSpeed.z *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);

	//�v���C���[�̏�Ԃ̕ύX�B
	if (m_playerState != enPlayerState_run) {
		if (stickL.Length() > 0.001f) {
			m_playerState = enPlayerState_move;
		}
		else {
			m_playerState = enPlayerState_idle;
		}
	}

	Shift();		//���Ⴊ�݂̏����B

	stickL = CVector3::Zero();
	moveSpeed = CVector3::Zero();
}

//�W�����v�B
void Player::Jump()
{
	if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {			//�T�o�C�o���̂Ƃ��B
		if (GetKeyDown(VK_SPACE) && m_characon.IsOnGround()) {		//�X�y�[�X����������B
			m_isJump = true;			//�W�����v�t���O��Ԃ��B
		}
		//�W�����v���̏����B
		if (m_isJump) {					

			moveSpeed.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity * gravitationalAcceleration;

			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity * gravitationalAcceleration) {
				m_isJump = false;
				m_jmpInitialVelocity = 3.f;
			}
		}
		else
		{
			//���R�����B
			if (!m_characon.IsOnGround()) {
				m_fallSpeed += 0.1f;
				moveSpeed.y -= m_gravity + m_fallSpeed;		//���R�����B
			}
			else
			{
				m_fallSpeed = 0.5f;
			}
		}
	}
}

//��]�����B
void Player::Turn()
{
	//�}�E�X�̈ړ��ʂ��擾�B
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetEngine().GetRealDeltaTimeSec();
	//��]����
	m_degreeY += mouseCursorMovePow.x;
	m_degreeXZ += mouseCursorMovePow.y;

	//XZ���̉�]�𐧌��B
	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//�}�E�X�̉�]�ʂ����W�A���ɕϊ��B
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	//��]���v�Z�B
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);

	m_skinModelRender->SetRot(modelRot);

	//�E�����Ɛ��ʕ����̃x�N�g���̌v�Z�B
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}

//���Ⴊ�݂̏����B
void Player::Shift()
{
	if (GetKeyInput(VK_SHIFT)) {
		m_playerState = enPlayerState_shift;
		//todo �u���b�N���痎���Ȃ�������ǉ�����B
	}
}

//�v���C���[�̏�ԊǗ��B
void Player::StateManagement()
{
	switch (m_playerState)
	{
	case Player::enPlayerState_idle:	//�ҋ@��ԁB

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_Idle, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_move:	//�ړ�(����)�B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		m_runSpeedDouble = 1.f;

		break;
	case Player::enPlayerState_run:		//�����Ă���Ƃ��B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.5f);
		m_runSpeedDouble = 2.f;

		break;
	case Player::enPlayerState_shift:	//���Ⴊ�݁B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_shift, 0.0f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		m_runSpeedDouble = 0.5f;

		break;
	case Player::enPlayerState_excavate:	//�����@��B

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_excavate, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		break;
	default:
		break;
	}
}