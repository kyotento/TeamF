#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "ItemData.h"

namespace {
	const float turnMult = 30.0f;			//プレイヤーの回転速度。
	const float maxDegreeXZ = 70.0f;		//XZ軸の回転の最大値。
	const float minDegreeXZ = -50.0f;		//XZ軸の回転の最小値。
	const float moveMult = 400.0f;			//プレイヤーの移動速度。

}

bool Player::Start()
{
	//プレイヤークラスの初期化。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.cmo");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(CVector3::One() * 0.001f);
	m_skinModelRender->SetRot(m_rotation);

	//キャラコンの初期化。
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
	//移動速度
	const float move = 1.0f;

	//WSADキーによる移動量
	CVector3 stickL = CVector3::Zero();

	//各キー入力により移動量を計算
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

	//左右入力の処理
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//上下入力の処理
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = 0.0f;
	moveSpeed *= moveMult * GetEngine().GetRealDeltaTimeSec();
	//キャラコンを移動させる。
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);
}

void Player::Turn()
{
	//向き
	//マウス
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetEngine().GetRealDeltaTimeSec();
	//回転処理
	m_degreeY += mouseCursorMovePow.x;
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

	//クォータニオンを計算
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);

	m_skinModelRender->SetRot(modelRot);

	//右方向と正面方向のベクトルの計算
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}
