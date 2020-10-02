#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
#include "ItemData.h"

bool Player::Start()
{
	m_model.Init(L"Resource/modelData/player.cmo");
	m_model.SetScale(CVector3::One() * 0.001f);

	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_position);

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
	Move();
	Turn();
}

void Player::Move()
{
	//�ړ����x
	const float moveMult = 400.0f;
	const float move = 1.0f;

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
	stickL.Normalize();

	CVector3 moveSpeed = CVector3::Zero();

	//���E���͂̏���
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//�㉺���͂̏���
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = 0.0f;
	moveSpeed *= moveMult * GetEngine().GetRealDeltaTimeSec();
	//�L�����R�����ړ�������B
	m_position = m_characon.Execute(moveSpeed);
	m_model.SetPos(m_position);
}

void Player::Turn()
{
	//��]���x
	const float turnMult = 30.0f;
	//XZ���̉�]�̐���
	const float maxDegreeXZ = 70.0f;
	const float minDegreeXZ = -50.0f;
	
	//����
	//�}�E�X
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetEngine().GetRealDeltaTimeSec();
	//��]����
	m_degreeY -= mouseCursorMovePow.x;
	m_degreeXZ += mouseCursorMovePow.y;

	//XZ���̉�]�𐧌�
	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//���W�A���ɕϊ�
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	//�N�H�[�^�j�I�����v�Z
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);

	m_model.SetRot(modelRot);

	//�E�����Ɛ��ʕ����̃x�N�g���̌v�Z
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}

void Player::PostRender()
{
	/*for (int i = 0; i < inventryWidth; i++) {
		if (m_itemList[i]->s_block->GetBlockType() != enCube_None) {
			//m_font
		}
	}*/
}