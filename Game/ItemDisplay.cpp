#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"

namespace {

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
	m_position = {0.0f,175.0f,0.0f};
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

	//MouseFollow();

	//追従処理。
	Follow();

	//回転
	Rotation();

	//切り替えの処理。
	Switching();

	m_skinModelRender->SetScale(m_scale);
}

//モデル生成。
void ItemDisplay::InitModel()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
	m_scale = { 0.35f,0.35f,0.35f };
	m_position = { 0.0f,175.0f,0.0f };
}

//モデルの追従。
void ItemDisplay::Follow()
{	
	const float pulsPosY = 40;					//Yは別で足してずらします。
	const float mullFornt = 15.0f;				//前ベクトルの数値を大きくする変数。
	const float mullCrossProduct = 30.0f;		//外積の数値を大きくする変数

	//次にプレイヤーの正面取得。
	m_forward = m_player->GetFront();
	m_forward *= mullFornt;

	//右方向にずらす。
	CVector3 right = m_player->GetRight();
	m_forward += (right * mullCrossProduct);
	
	//下方向にずらす。
	m_forward.y -= pulsPosY;

	//上下方向に回転させる。
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//カメラの座標取得。
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	////座標をモデルへ。
	m_skinModelRender->SetPos(m_position);
}

//切り替え（まだ中身作らないです。）
void ItemDisplay::Switching()
{

}

//腕の回転処理
void ItemDisplay::Rotation()
{
	const float m_rotX = 25.0f;					//前に倒すための変数。

	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();

	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(),-m_rotX);
	m_rotation.Multiply(m_rotationX);
	//ここでプレイヤーの上下にそってずらします。
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
}