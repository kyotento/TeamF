#include "stdafx.h"
#include "Player.h"
#include <assert.h>
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h>

namespace {
	float characonRadius = 50.f;			//キャラコンの半径。
	float characonHeight = 100.f;			//キャラコンの高さ。

	float moveSpeed = 20.f;					//プレイヤーの移動速度。
	float mouseSensitivity = 1.0f;			//マウス感度。
	float degry;							//マウスの移動量を加算したもの。
	float radian;

	CVector2 mouseCursorMovePow = CVector2::Zero();		//マウスカーソルの移動量。

	CVector3 movePower;						//キャラコンの移動にかかる力。
	CVector3 moveSp;

	CQuaternion rotMousePower;				//マウスの回転量。
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
	//プレイヤーモデルの初期化。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.cmo");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);

	m_skn = NewGO<GameObj::CSkinModelRender>();
	m_skn->Init(L"Resource/modelData/flore_kari.cmo");
	m_skn->SetPos({ 0.f,0.f,0.f });

	m_charaPos = m_position;
	//キャラコンの初期化。
	m_characon.Init(characonRadius, characonHeight, m_charaPos);

	return true;
}

void Player::Update()
{
	Move();			//移動処理。
	Rotation();		//回転処理。
}

//移動処理。
void Player::Move()
{
	//キーボードの移動処理。
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
	//スティックの上下入力の処理
	moveSp.z += cos(radian) * movePower.z * moveSpeed;
	moveSp.x += sin(radian) * movePower.z * moveSpeed;
	moveSp.y = 0.0f;

	//キャラコンを移動させる。
	m_position = m_characon.Execute(moveSp, 1.f);

	m_skinModelRender->SetPos(m_position);		//プレイヤーの座標を指定する。

	movePower = CVector3::Zero();				//キャラコンの移動量を初期化。

}

//回転処理。
void Player::Rotation()
{
	mouseCursorMovePow = MouseCursor().GetMouseMove();			//マウスの移動量を取得。

	degry += mouseCursorMovePow.x * mouseSensitivity;			//マウスの向き。

	rotMousePower.SetRotationDeg(CVector3::AxisY(), mouseCursorMovePow.x * mouseSensitivity);		//移動量をもとにY軸回転の力を計算。

	m_rot.Multiply(rotMousePower);			//元のRotationに加算。

	m_skinModelRender->SetRot(m_rot);

}