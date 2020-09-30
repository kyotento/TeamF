#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 

void Player::Update()
{
	Move();
	Turn();
}

void Player::Move()
{
	const float mult = 4.0f;
	
	//左スティックの入力量を取得
	CVector3 stickL;
	stickL.y = -Pad(0).GetStick(L).y;
	stickL.x = Pad(0).GetStick(L).x;	//アナログスティックの入力量を取得。

	CVector3 moveSpeed = CVector3::Zero();

	moveSpeed.z = sin(m_radian)*stickL.x * mult;
	moveSpeed.x = -cos(m_radian)*stickL.x * mult;
	//スティックの上下入力の処理
	moveSpeed.z += cos(m_radian)*stickL.y * mult;
	moveSpeed.x += sin(m_radian)*stickL.y * mult;
	moveSpeed.y = 0.0f;
	m_position += moveSpeed * mult / GetEngine().GetStandardFrameRate();
}

void Player::Turn()
{
	const float Mult = 60.0f;

	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}

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

	CVector3 moveSpeedXZ = { 0.0f,0.0f,1.0f };
	m_rotation.Multiply(moveSpeedXZ);
	m_right = moveSpeedXZ;
}