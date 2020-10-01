#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
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

	//左スティックの入力量を取得
	CVector3 stickL;
	if (m_gameCamera->GetCameraMode() == EnMode_ReverseTPS) {
		stickL.y = Pad(0).GetStick(L).y;
		stickL.x = -Pad(0).GetStick(L).x;	//アナログスティックの入力量を取得。
	}
	else {
		stickL.y = -Pad(0).GetStick(L).y;
		stickL.x = Pad(0).GetStick(L).x;	//アナログスティックの入力量を取得。
	}

	CVector3 moveSpeed = CVector3::Zero();

	/*if (m_gameCamera->GetIsFPS()) {
		stickL.y = Pad(0).GetStick(L).y;
		stickL.x = -Pad(0).GetStick(L).x;	//アナログスティックの入力量を取得。
	}
	else {
		stickL.y = -Pad(0).GetStick(L).y;
		stickL.x = Pad(0).GetStick(L).x;	//アナログスティックの入力量を取得。
	}*/
	moveSpeed.z = sin(m_radian)*stickL.x * mult;
	moveSpeed.x = -cos(m_radian)*stickL.x * mult;
	//スティックの上下入力の処理
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
	//自機の角度の差分
	m_radian = M_PI / 180 * m_degree;
	//右スティック
	CVector2 stickR;
	stickR.x = -Pad(0).GetStick(R).x;	//アナログスティックのxの入力量を取得。
	//向き
	//右スティック
 	float sDegree = -stickR.x * Mult / GetEngine().GetStandardFrameRate();
	//回転処理
	m_degree += sDegree;
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degree);

	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);

	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}