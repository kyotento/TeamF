#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"

namespace {
	const float m_pulsPosY = 75.0f;				//Yは別で足してずらします。
	const float m_playerPulsPosX = 1.0f;		//Xはプレイヤーのほうに足します。
	const float m_pulsPosZ = 50.0f;			//こいつを実験台にしてます。
	const float m_mullPos = 50.0f;				//ひとまず前ベクトルの数値を大きくする変数。
	float m_degreeY = 0.0f;
	const float maxDegreeY = 70.0f;				//Y軸の回転の最大値。
	const float minDegreeY = -50.0f;			//Y軸の回転の最小値。

	CVector3 neerPosition = CVector3::Zero();	//仮pos。
}

ItemDisplay::ItemDisplay()
{
}

ItemDisplay::~ItemDisplay()
{
	DeleteGO(m_skinModelRender);
}

bool ItemDisplay::Start()
{
	//モデル生成
	InitModel();
	return true;
}

void ItemDisplay::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	//追従処理。
	Follow();

	//回転
	Rotation();

	//切り替えの処理。
	Switching();
}

//モデル生成。
void ItemDisplay::InitModel()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
}

//モデルの追従。
void ItemDisplay::Follow()
{
	//マウスの移動量を取得。
	const float turnMult = 20.0f;
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetDeltaTimeSec();
	//回転処理
	m_degreeY += mouseCursorMovePow.x;
	//XZ軸の回転を制限。
	if (m_degreeY < minDegreeY) {
		m_degreeY = minDegreeY;
	}
	else if (m_degreeY > maxDegreeY) {
		m_degreeY = maxDegreeY;
	}
	//m_forward = m_gameCamera->GetPos();
	//auto m_rot = CMatrix::Identity();
	//m_rot.MakeRotationFromQuaternion(m_rotation);
	//m_forward.x = m_rot.m[2][0];
	//m_forward.y = m_rot.m[2][1];
	//m_forward.z = m_rot.m[2][2];

	//みぎ前方に置きたい。
	m_forward = m_player->GetFront();
	m_forward *= m_mullPos;
	m_forward.y += m_pulsPosY;
	m_forward.z += m_pulsPosZ;
	//足してます。
	m_position = m_position + m_forward;
	//座標をモデルへ。
	m_skinModelRender->SetPos(m_position);
}

//切り替え（まだ中身作らないです。）
void ItemDisplay::Switching()
{

}

//腕の回転処理
void ItemDisplay::Rotation()
{
	//モデルを回転させるよ。
	m_rotation = m_player->GetRot();
	//m_rotation.SetRotationDeg(CVector3::AxisY(),45.0f);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
}