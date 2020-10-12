#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "ItemData.h"
#include "Chunk.h"

namespace {
	const float turnMult = 30.0f;			//プレイヤーの回転速度。
	const float maxDegreeXZ = 70.0f;		//XZ軸の回転の最大値。
	const float minDegreeXZ = -50.0f;		//XZ軸の回転の最小値。
	const float moveMult = 40.0f;			//プレイヤーの移動速度。
	const float move = 1.0f;				//移動速度(基本的には触らない)。

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
	//プレイヤークラスの初期化。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Assets/modelData/player.tkm", m_animationClip, enAnimationClip_Num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(CVector3::One() * 0.001f * Block::WIDTH);
	m_skinModelRender->SetRot(m_rotation);

	//キャラコンの初期化。
	m_characon.Init(m_characonRadius, m_characonHeight, m_position);

	//インベントリ―の初期化。
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
	//回転処理。
	Turn();
	//プレイヤーの状態管理。0
	StateManagement();
}

//移動処理。
void Player::Move()
{
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
	if( GetKeyInput( VK_SHIFT ) ){
		stickL.z = -move;
	} else if( GetKeyInput( VK_SPACE ) ){
		stickL.z = move;
	}
	stickL.Normalize();

	CVector3 moveSpeed = CVector3::Zero();		//プレイヤーの移動速度(方向もち)。

	//左右入力の処理
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//上下入力の処理
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = stickL.z;
	moveSpeed *= moveMult * GetEngine().GetStandardFrameRate();
	//キャラコンを移動させる。
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);

	//プレイヤーの状態の変更。
	if (stickL.Length() > 0.001f) {
		m_playerState = enPlayerState_move;
	}
	else {
		m_playerState = enPlayerState_idle;
	}
}

//回転処理。
void Player::Turn()
{
	//マウスの移動量を取得。
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetEngine().GetRealDeltaTimeSec();
	//回転処理
	m_degreeY += mouseCursorMovePow.x;
	m_degreeXZ += mouseCursorMovePow.y;

	//XZ軸の回転を制限。
	if (m_degreeXZ < minDegreeXZ) {
		m_degreeXZ = minDegreeXZ;
	}
	else if (m_degreeXZ > maxDegreeXZ) {
		m_degreeXZ = maxDegreeXZ;
	}

	//マウスの回転量をラジアンに変換。
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	//回転を計算。
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);

	m_skinModelRender->SetRot(modelRot);

	//右方向と正面方向のベクトルの計算。
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}

//プレイヤーの状態管理。
void Player::StateManagement()
{
	switch (m_playerState)
	{
	case Player::enPlayerState_idle:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_Idle, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_move:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_excavate:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_excavate, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		break;
	default:

		break;
	}
}