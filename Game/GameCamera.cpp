#include "stdafx.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "Player.h"

namespace {
	constexpr float NORMAL_VIEW_ANGLE_DEG = 90.0f;//通常時の視野角(度)
	constexpr float DASH_VIEW_ANGLE_DEG = 110.0f;//ダッシュ時の視野角(度)
	constexpr float VIEW_ANGLE_CHANGE_DEG_PER_SEC = 80.0f;//視野角が変化する一秒間の角度
	constexpr float ROLL_MODORI_DEG_PER_SEC = 300.0f;//ロール回転がもとに戻る一秒間の角度
}

bool GameCamera::Start()
{
	m_position = CVector3(00.0f, 20.0f, 10.0f);
	m_camera = NewGO<GameObj::PerspectiveCamera>();
	m_target = CVector3(0.0f, 10.0f, 0.0f);
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
	m_camera->SetNear(10.f);
	m_camera->SetFar( 50000 );
	m_camera->SetViewAngleDeg(m_viewAngleDeg = NORMAL_VIEW_ANGLE_DEG);
	SetMainCamera(m_camera);
	return true;
}

void GameCamera::Update()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}

	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	m_radianXZ = m_player->GetRadianXZ();

	//ダッシュ中は視野角が広がる
	if (m_player->GetplayerState() == Player::enPlayerState::enPlayerState_run) {
		m_viewAngleDeg = min(m_viewAngleDeg + GetDeltaTimeSec() * VIEW_ANGLE_CHANGE_DEG_PER_SEC, DASH_VIEW_ANGLE_DEG);
	}
	else {
		m_viewAngleDeg = max(m_viewAngleDeg - GetDeltaTimeSec() * VIEW_ANGLE_CHANGE_DEG_PER_SEC * 2.0f, NORMAL_VIEW_ANGLE_DEG);
	}
	m_camera->SetViewAngleDeg(m_viewAngleDeg);

	//カメラロール回転
	if (!m_isRollLock) {
		//減衰
		float rollPow = ROLL_MODORI_DEG_PER_SEC * GetDeltaTimeSec();
		if (abs(m_rollDeg) <= rollPow) {
			m_rollDeg =-0.0f;
		}
		else {
			m_rollDeg = (abs(m_rollDeg) - rollPow) * (m_rollDeg < 0.0f ? -1.0f : 1.0f);
		}
	}
	{
		//回転
		CQuaternion rot;
		rot.SetRotationDeg(m_camera->GetFront(), m_rollDeg);
		CVector3 up = CVector3::Up();
		rot.Multiply(up);
		m_camera->SetUp(up);
	}

	//カメラのモードに応じて処理を分岐
	switch (m_mode)
	{
	case EnMode_FPS:
		FPS();
		break;
	case EnMode_TPS:
		TPS();
		break;
	case EnMode_ReverseTPS:
		ReverseTPS();
		break;
	default:
		break;
	}

	//F5が押されたらカメラのモードを切り替える
	if (GetKeyDown(116)) {
		m_mode = EnCameraMode(m_mode + 1);
		if (m_mode == EnMode_Num) {
			m_mode = EnMode_FPS;
		}
	}

}

void GameCamera::FPS()
{
	//カメラの高さ
	const float height = 1.50f* Block::WIDTH;
	//カメラをプレイヤーの正面に
	const float front = 0.05f* Block::WIDTH;

	m_position = m_player->GetModelPos();
	m_position += m_player->GetFront() * front;
	m_position.y += height;
	//Y軸周りに回転させる。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), m_radianY);
	CVector3 toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(toPos);
	//上下の回転。
	//まずは回す軸を計算する。
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	//ベクトルを正規化する。
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, m_radianXZ);
	qRot.Multiply(toPos);
	toPos *= m_radius*1.5;
	m_target = m_position - toPos;
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
}

void GameCamera::TPS()
{
	//カメラの高さ
	const float height = 0.20f* Block::WIDTH;
	//カメラをプレイヤーより横にずらす
	const float right = 0.20f* Block::WIDTH;
	
	m_target = m_player->GetModelPos() + m_player->GetRight() * right;
	m_target.y += height;
	//Y軸周りに回転させる。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), m_radianY);
	CVector3 toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(toPos);
	//上下の回転。
	//まずは回す軸を計算する。
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	//ベクトルを正規化する。
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, m_radianXZ);
	qRot.Multiply(toPos);
	toPos *= m_radius;
	m_position = m_target + toPos;
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
}

void GameCamera::ReverseTPS()
{
	//カメラの高さ
	const float height = 0.1f* Block::WIDTH;
	//カメラをプレイヤーの正面に
	const float front = 0.05f* Block::WIDTH;

	m_position = m_player->GetModelPos();
	m_position += m_player->GetFront() * front;
	m_position.y += height;
	//Y軸周りに回転させる。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), m_radianY);
	CVector3 toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(toPos);
	//上下の回転。
	//まずは回す軸を計算する。
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	//ベクトルを正規化する。
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, m_radianXZ);
	qRot.Multiply(toPos);
	toPos *= m_radius;
	m_target = m_position - toPos;
	m_camera->SetPos(m_target);
	m_camera->SetTarget(m_position);
}