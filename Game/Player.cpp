#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "ItemData.h"

bool Player::Start()
{
	m_model.Init(L"Resource/modelData/GrassBlock.cmo");
	m_model.SetScale(CVector3::One() * 0.01f);

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
	//移動速度
	const float mult = 10.0f;

	//WSADキーによる移動量
	CVector3 stickL = CVector3::Zero();

	bool isKey = false;
	if (GetKeyInput('W')) {
		stickL.y = -1.0f;
		isKey = true;
	}
	else if (GetKeyInput('S')) {
		stickL.y = 1.0f;
		isKey = true;
	}
	if (GetKeyInput('A')) {
		stickL.x = -1.0f;
		isKey = true;
	}
	else if (GetKeyInput('D')) {
		stickL.x = 1.0f;
		isKey = true;
	}
	if (isKey) {
		stickL.Normalize();
	}

	CVector3 moveSpeed = CVector3::Zero();

	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = 0.0f;
	m_position += moveSpeed * mult / GetEngine().GetStandardFrameRate();
	m_model.SetPos(m_position);
}

void Player::Turn()
{
	//回転速度
	const float Mult = 30.0f;
	//XZ軸の回転の制限
	const float maxDegreeXZ = 70.0f;
	const float minDegreeXZ = -50.0f;
	
	//向き
	//マウス
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * Mult * GetEngine().GetRealDeltaTimeSec();
	//回転処理
	m_degreeY -= mouseCursorMovePow.x;
	m_degreeXZ += mouseCursorMovePow.y;

	//XZ軸の回転を制限
	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//ラジアンに変換
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);

	//右方向と正面方向のベクトルの計算
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