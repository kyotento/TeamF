#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "ItemData.h"
#include "Chunk.h"
#include "GameMode.h"

namespace {
	const float turnMult = 30.0f;			//プレイヤーの回転速度。
	const float maxDegreeXZ = 70.0f;		//XZ軸の回転の最大値。
	const float minDegreeXZ = -50.0f;		//XZ軸の回転の最小値。
	const float moveMult = 8.0f;			//プレイヤーの移動速度。
	const float move = 1.0f;				//移動速度(基本的には触らない)。
	const float gravitationalAcceleration = 0.3f;		//重力加速度。
	CVector3 stickL = CVector3::Zero();		//WSADキーによる移動量
	CVector3 moveSpeed = CVector3::Zero();		//プレイヤーの移動速度(方向もち)。

}

Player::Player()
{
	//アニメーションの設定。
	m_animationClip[enAnimationClip_Idle].Load(L"Assets/animData/player_idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_move].Load(L"Assets/animData/player_move.tka");
	m_animationClip[enAnimationClip_move].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shift].Load(L"Assets/animData/player_shift.tka");
	m_animationClip[enAnimationClip_shift].SetLoopFlag(true);
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
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
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

//キーボードの入力情報管理。
void Player::KeyBoardInput()
{
	Dash();		//走る処理。

	//各キー入力により移動量を計算
	//if (!m_doubleCilckFlag) {
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

	if (m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {		//クリエイティブのとき。
		if (GetKeyInput(VK_SHIFT)) {
			stickL.z = -move;
		}
		else if (GetKeyInput(VK_SPACE)) {
			stickL.z = move;
		}
	}
}

//走る処理。
void Player::Dash()
{
	if (GetKeyUp('W')) {
		m_doubleCilckFlag = true;
	}
	if (m_doubleCilckFlag) {
		if (m_doubleClickTimer <= 0.2f && m_playerState != enPlayerState_run) {
			m_doubleClickTimer += GetEngine().GetRealDeltaTimeSec();	//タイマーを進める。
		}
		if (m_doubleClickTimer <= 0.2f) {
			if (GetKeyDown('W')) {		//走らせる。
				m_playerState = enPlayerState_run;
				m_runFlag = true;
			}

			if (GetKeyUp('W') && m_runFlag) {
				m_playerState = enPlayerState_idle;		//走るのをやめる。
				m_doubleClickTimer = 0.f;
				m_doubleCilckFlag = false;
				m_runFlag = false;
			}
		}
		else {			//一定時間経過した時。
			m_playerState = enPlayerState_idle;
			m_doubleClickTimer = 0.f;
			m_doubleCilckFlag = false;
		}
	}
}

//移動処理。
void Player::Move()
{
	KeyBoardInput();		//キーボードの入力情報。
	stickL.Normalize();

	//左右入力の処理
	moveSpeed.z = sin(m_radianY) * stickL.x;
	moveSpeed.x = -cos(m_radianY) * stickL.x;
	//上下入力の処理
	moveSpeed.z += cos(m_radianY) * stickL.y;
	moveSpeed.x += sin(m_radianY) * stickL.y;
	moveSpeed.y = stickL.z;

	Jump();   //ジャンプ処理。

	moveSpeed.x *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	moveSpeed.y *= moveMult * GetEngine().GetStandardFrameRate();
	moveSpeed.z *= moveMult * GetEngine().GetStandardFrameRate() * m_runSpeedDouble;
	//キャラコンを移動させる。
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position);

	//プレイヤーの状態の変更。
	if (m_playerState != enPlayerState_run) {
		if (stickL.Length() > 0.001f) {
			m_playerState = enPlayerState_move;
		}
		else {
			m_playerState = enPlayerState_idle;
		}
	}

	Shift();		//しゃがみの処理。

	stickL = CVector3::Zero();
	moveSpeed = CVector3::Zero();
}

//ジャンプ。
void Player::Jump()
{
	if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {			//サバイバルのとき。
		if (GetKeyDown(VK_SPACE) && m_characon.IsOnGround()) {		//スペースを押したら。
			m_isJump = true;			//ジャンプフラグを返す。
		}
		//ジャンプ中の処理。
		if (m_isJump) {					

			moveSpeed.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity * gravitationalAcceleration;

			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity * gravitationalAcceleration) {
				m_isJump = false;
				m_jmpInitialVelocity = 3.f;
			}
		}
		else
		{
			//自由落下。
			if (!m_characon.IsOnGround()) {
				m_fallSpeed += 0.1f;
				moveSpeed.y -= m_gravity + m_fallSpeed;		//自由落下。
			}
			else
			{
				m_fallSpeed = 0.5f;
			}
		}
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

//しゃがみの処理。
void Player::Shift()
{
	if (GetKeyInput(VK_SHIFT)) {
		m_playerState = enPlayerState_shift;
		//todo ブロックから落ちない処理を追加する。
	}
}

//プレイヤーの状態管理。
void Player::StateManagement()
{
	switch (m_playerState)
	{
	case Player::enPlayerState_idle:	//待機状態。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_Idle, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);

		break;
	case Player::enPlayerState_move:	//移動(歩き)。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		m_runSpeedDouble = 1.f;

		break;
	case Player::enPlayerState_run:		//走っているとき。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.5f);
		m_runSpeedDouble = 2.f;

		break;
	case Player::enPlayerState_shift:	//しゃがみ。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_shift, 0.0f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		m_runSpeedDouble = 0.5f;

		break;
	case Player::enPlayerState_excavate:	//物を掘る。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_excavate, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.0f);
		break;
	default:
		break;
	}
}