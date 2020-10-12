#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "ItemData.h"
#include "Chunk.h"

namespace {
	const float turnMult = 30.0f;			//�v���C���[�̉�]���x�B
	const float maxDegreeXZ = 70.0f;		//XZ���̉�]�̍ő�l�B
	const float minDegreeXZ = -50.0f;		//XZ���̉�]�̍ŏ��l�B
	const float moveMult = 40.0f;			//�v���C���[�̈ړ����x�B
	const float move = 1.0f;				//�ړ����x(��{�I�ɂ͐G��Ȃ�)�B

}

Player::Player()
{
	m_animationClip[enAnimationClip_Idle].Load(L"Assets/animData/player_idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_move].Load(L"Assets/animData/player_move.tka");
	m_animationClip[enAnimationClip_move].SetLoopFlag(true);
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
	m_skinModelRender->SetScale(CVector3::One() * 0.001f * Block::WIDTH);
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

//�ړ������B
void Player::Move()
{
	//WSAD�L�[�ɂ��ړ���
	CVector3 stickL = CVector3::Zero();

	//�e�L�[���͂ɂ��ړ��ʂ��v�Z
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
	if( GetKeyInput( VK_SHIFT ) ){
		stickL.z = -move;
	} else if( GetKeyInput( VK_SPACE ) ){
		stickL.z = move;
	}
	stickL.Normalize();

	CVector3 moveSpeed = CVector3::Zero();		//�v���C���[�̈ړ����x(��������)�B

	//���E���͂̏���
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//�㉺���͂̏���
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = stickL.z;
	moveSpeed *= moveMult * GetEngine().GetStandardFrameRate();
	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);

	//�v���C���[�̏�Ԃ̕ύX�B
	if (stickL.Length() > 0.001f) {
		m_playerState = enPlayerState_move;
	}
	else {
		m_playerState = enPlayerState_idle;
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

//�v���C���[�̏�ԊǗ��B
void Player::StateManagement()
{
	switch (m_playerState)
	{
	case Player::enPlayerState_idle:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_Idle, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_move:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_excavate:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_excavate, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		break;
	default:

		break;
	}
}