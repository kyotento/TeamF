#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "Item.h"
#include "GameMode.h"
#include "World.h"
#include "PlayerInventory.h"
#include "BlockFactory.h"
#include "DamegeScreenEffect.h"
#include "Enemy.h"
#include "PlayerParameter.h"
#include "PlayerDeath.h"
#include "Menu.h"
#include "DropItem.h"
#include "Animals.h"
#include "PlayerArmor.h"
#include "NullableItemStack.h"
#include "CalcMuki.h"
#include "PlayerDataFiler.h"

namespace {
	constexpr float characonRadius = 50.f;					//キャラコンの半径。
	constexpr float characonHeight = 160.f;					//キャラコンの高さ。
	constexpr float BLOCK_BREAK_START_HEIGHT = ((characonHeight + characonRadius * 2.0f) * 0.5f);
	constexpr float turnMult = 20.0f;						//プレイヤーの回転速度。
	constexpr float maxDegreeXZ = 88.0f;					//XZ軸の回転の最大値。
	constexpr float minDegreeXZ = -88.0f;					//XZ軸の回転の最小値。
	constexpr float moveMult = 8.0f;						//プレイヤーの移動速度。
	constexpr float move = 2.f;								//移動速度(基本的には触らない)。
	constexpr float gravitationalAcceleration = 0.3f;		//重力加速度。
	constexpr float doubleClickRug = 0.15f;					//ダブルクリック判定になる間合い。
	constexpr float timeBlockDestruction = 0.3f;			//ブロック破壊の時間制限
	int fallTimer = 0;									//滞空時間。
	int hiddenStamina = 0;								//体力回復用の隠れスタミナ。
	float staminaTimer = 0.f;							//隠れスタミナ消費による体力回復。
	float hungryDamageTimer = 0.f;						//空腹ダメージのタイマー。
	constexpr int HAND_ATTACK_POW = 2;					//素手の攻撃力。
	constexpr float WALK_SOUND_TIME = 0.5f;				//足音鳴らす間隔。

	bool isStrikeFlag = true;
	bool isBlockDestroy = false;
	CVector3 stickL = CVector3::Zero();					//WSADキーによる移動量
	CVector3 moveSpeed = CVector3::Zero();				//プレイヤーの移動速度(方向もち)。
	CVector3 itemDisplayPos = CVector3::Zero();			//アイテム（右手部分）の位置。
	constexpr int randomDrop = Block::WIDTH / 0.5f;		//らんちゅうのはんい。
	std::mt19937 random((std::random_device())());		//らんちゅう。
}
					//装備スロットのため拡張。
Player::Player() : m_inventory(40)//, Entity(enEntity_None, true)
{
	//アニメーションの設定。
	m_animationClip[enAnimationClip_Idle].Load(L"Resource/animData/player_idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_move].Load(L"Resource/animData/player_move.tka");
	m_animationClip[enAnimationClip_move].SetLoopFlag(true);
	m_animationClip[enAnimationClip_excavate].Load(L"Resource/animData/player_Excavate.tka");
	m_animationClip[enAnimationClip_excavate].SetLoopFlag(true);
}

Player::~Player()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_playerParameter);
	DeleteGO(m_playerDeath);
	DeleteGO(m_playerArmor);

	//プレイヤーデータの保存。
	PlayerDataFiler playerFiler;
	playerFiler.Save( this );
}

#include "ItemStack.h"
bool Player::Start()
{
	//プレイヤーデータ読み込み。
	PlayerDataFiler playerFiler;
	playerFiler.Load( this );

	//プレイヤークラスの初期化。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.tkm", m_animationClip, enAnimationClip_Num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetRot(m_rotation);
	//レイトレモデル初期化
	m_raytraceModel.Init(*m_skinModelRender);

	m_damageName = L"Resource/soundData/player/damage.wav";
	m_attackName = L"Resource/soundData/player/attack.wav";
	m_putName = L"Resource/soundData/player/put.wav";
	m_walkName = L"Resource/soundData/player/walk.wav";
	m_strikeName = L"Resource/soundData/player/strike.wav";
	//キャラコンの初期化。
	m_characon.Init(characonRadius, characonHeight, m_position);
	m_characon.SetIsDrawCollider(true);

	//被弾判定用コリジョン。
	m_damageCollision = std::make_unique<SuicideObj::CCollisionObj>();
	CVector3 colPos = (m_position.x, m_position.y + Block::WIDTH, m_position.z);		//コリジョン座標。
	m_damageCollision->CreateCapsule(colPos, m_rotation, characonRadius, characonHeight);
	m_damageCollision->SetTimer(enNoTimer);				//寿命無し。
	m_damageCollision->SetName(L"CPlayer");
	m_damageCollision->SetClass(this);					//クラスのポインタを取得。
	m_damageCollision->SetIsHurtCollision(true);		//自分から判定をとらない。

	//ブロック破壊モデル
	m_blockCrackModel.Init(L"Resource/modelData/crack.tkm");
	m_blockCrackModel.SetIsDraw(false);
	//m_blockCrackModel.InitPostDraw(PostDrawModelRender::enBlendMode::enAlpha);

	//TODO: デバッグ専用
	//プレイヤーのインベントリ情報がロードできなかったら。
	if (playerFiler.IsLoadSuccess() == false) {
		//プレイヤーにテスト用アイテムを持たせる。
		int itemArray[] = { enCube_SandStone,enCube_OakWood };
		for (int i : itemArray) {
			auto item = std::make_unique<ItemStack>(Item::GetItem(i), Item::GetItem(i).GetStackLimit());
			m_inventory.AddItem(item);
		}
	}

	//プレイヤーのパラメーター生成。
	m_playerParameter = NewGO<PlayerParameter>();
	m_playerParameter->SetPlayerIns(this);

	//アーマークラス生成。
	m_playerArmor = NewGO<PlayerArmor>();
	m_playerArmor->SetPlayerSkinModel(m_skinModelRender);
	m_playerArmor->SetPlayer(this);

	//タイマーに値を入れておく
	m_timerBlockDestruction = timeBlockDestruction;
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
	//EscMenuが開かれているとき処理をしない。
	if (m_game->GetIsEscMenu()) {
		return;
	}

	//骨を取得。
	m_headBone = m_skinModelRender->FindBone(L"Bone002");
	m_shoulderBone = m_skinModelRender->FindBone(L"Bone008");
	m_rightHandBone = m_skinModelRender->FindBone(L"Bone009");
	//死んでないとき。
	if (m_playerState != enPlayerState_death) {
		//移動処理。GUIが開かれているとき、入力は遮断しているが、重力の処理は通常通り行う。
		Move();
		m_deathFlag = false;
		//GUIが開かれている場合には、回転とインベントリを開くことは行わない。
		if (m_openedGUI == nullptr) {
			//回転処理。
			Turn();
			//攻撃。
			Attack();
			//インベントリを開く。
			OpenInventory();
			//ブロック破壊を実行するかどうか判断する。
			DecideCanDestroyBlock();
			//前方にRayを飛ばす。
			FlyTheRay();
			//スタミナ処理。
			Stamina();
			//ノックバック。
			KnockBack();
			//アイテムを投げる処理。
			ThrowItem();
		}
		else if (GetKeyDown('E')) {
			//GUIが開かれているときに、Eが押されたらGUIを閉じる。
			CloseGUI();
		}
		if (m_openedGUI != nullptr) {
			MouseCursor().SetLockMouseCursor(false);		//マウスカーソルの固定を外す。
			m_eatingFlag = false;
		}
	}
	if (m_isExpUpFlag)
	{
		SuicideObj::CSE* upse;
		upse = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/expget.wav");
		upse->SetVolume(1.0f);
		upse->Play();
		m_isExpUpFlag = false;
	}
	//プレイヤーの状態管理。
	StateManagement();
	//死亡処理。
	Death();
	//モデルを描画するかどうか。
	IsDraw();
	//防御力きめるー。
	Defence();
	//攻撃力も。
	CalcAttackPow();
	//空腹ダメージ。
	HungryDamage();

	//奈落死。
	if (m_position.y <= 0.f) {
		TakenDamage(1, 0.0f, false, true);
	}
}

void Player::SetRadianY( float rot ){
	m_radianY = rot;
	m_degreeY = rot / M_PI * 180.0f;
}

void Player::SetRadianXZ( float rot ){
	m_radianXZ = rot;
	m_degreeXZ = rot / M_PI * 180.0f;
}

inline void Player::OpenGUI( std::unique_ptr<GUI::RootNode>&& gui ){
	m_openedGUI = std::move( gui );
	MouseCursor().SetLockMouseCursor( false );		//マウスカーソルの固定を外す。
}

inline void Player::CloseGUI(){
	m_openedGUI.reset();
	MouseCursor().SetLockMouseCursor( true );		//マウスカーソルを固定する。
}

//キーボードの入力情報管理。
void Player::KeyBoardInput()
{
	//開いているGUIがあれば入力を遮断する。
	if( m_openedGUI ){
		stickL = CVector3::Zero();
		return;
	}

	Dash();		//走る処理。

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

	if (m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {		//クリエイティブのとき。

		ChangeMovemontC();

		if (m_flyingMode) {			//飛行モードのとき。
			if (GetKeyInput(VK_SHIFT)) {
				stickL.z = -move;
			}
			else if (GetKeyInput(VK_SPACE)) {
				stickL.z = move;
			}
		}
	}
}

//スペースをダブルクリックしたかどうか。
bool Player::SpaceDoubleClick()
{
	bool doubleClickeFlag = false;
	if (!m_doubleClickFlagC) {
		if (GetKeyUp(VK_SPACE)) {			//スペースキーを離したとき。
			m_doubleClickFlagC = true;
		}
	}
	if (m_doubleClickFlagC) {
		if (m_doubleClickTimerC <= doubleClickRug) {
			m_doubleClickTimerC += GetEngine().GetRealDeltaTimeSec();
			if (GetKeyDown(VK_SPACE)) {
				doubleClickeFlag = true;
			}
		}
		else {
			m_doubleClickFlagC = false;
			m_doubleClickTimerC = 0.f;
			doubleClickeFlag = false;
		}
	}	
	return doubleClickeFlag;
}

// 移動方法の切り替え(クリエイティブ)。
void Player::ChangeMovemontC()
{
	if (SpaceDoubleClick()) {		//ダブルクリックしたら。
		if (!m_flyingMode){
			m_flyingMode = true;	//飛行モードに。
			return;
		}
		else {						//ダブルクリックしていなければ。
			m_flyingMode = false;	//歩行モードに。
		}
	}
}

//走る処理(2種類あります)。
void Player::Dash()
{
	//Wダブルクリック。
	if (GetKeyUp('W')) {
		m_doubleCilckFlag = true;
	}
	if (m_playerState == enPlayerState_KnockBack)
	{
		return;
	}
	if (m_doubleCilckFlag) {
		if (m_doubleClickTimer <= doubleClickRug && m_playerState != enPlayerState_run) {
			m_doubleClickTimer += GetEngine().GetRealDeltaTimeSec();	//タイマーを進める。
		}
		if (m_doubleClickTimer <= doubleClickRug) {
			if (GetKeyDown('W') && m_stamina > 6) {		//走らせる。
				m_playerState = enPlayerState_run;
				m_runFlag = true;
			}
			
			if ((GetKeyUp('W') || m_stamina <= 6) && m_runFlag) {
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

	//Ctrl+W。
	if (GetKeyInput('W') && GetKeyInput(VK_CONTROL) && m_stamina > 6) {
		m_playerState = enPlayerState_run;
	}
	if (m_playerState == enPlayerState_run) {
		if (GetKeyUp('W') || GetKeyUp(VK_CONTROL) || m_stamina <= 7) {
			m_playerState = enPlayerState_move;
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
	if (m_flyingMode			//クリエイティブの飛行モードなら移動速度を上げる。
		&& m_gameMode->GetGameMode() == GameMode::enGameModeCreative) {			
		moveSpeed *= m_creativeSpeedMag;
	}
	//キャラコンを移動させる。
	m_position = m_characon.Execute(moveSpeed);
	m_skinModelRender->SetPos(m_position + CVector3::Down()*(GetIsSneaking() ? Block::WIDTH/3.f : 0.0f));
	//ダメージ当たり判定移動。
	CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//当たり判定の座標。
	m_damageCollision->SetPosition(colPos);

	//足音
	m_walkingTimer += GetDeltaTimeSec();
	if (m_characon.IsOnGround() && m_walkingTimer > WALK_SOUND_TIME / m_runSpeedDouble && stickL.Length() > 0.001f) {
		auto walk = NewGO<SuicideObj::CSE>(m_walkName);
		walk->SetVolume(0.125f);
		walk->Play();
		m_walkingTimer = 0.0f;
	}

	//プレイヤーの状態の変更。
	if (m_playerState != enPlayerState_run && m_playerState != enPlayerState_KnockBack) {
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
	if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival		//サバイバルのときか。
		|| m_flyingMode == false) {										//クリエイティブのフライモードでないとき。
		if (GetKeyInput(VK_SPACE) && m_characon.IsOnGround() && m_openedGUI == nullptr) {	//スペースが押されていたら&&地面にいたら&& GUIが未表示なら。
			m_isJump = true;			//ジャンプフラグを返す。
			if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
				m_stamina -= 0.2f;			//サバイバルモードの時のみスタミナを減らす。
			}
		}
		//ジャンプ中の処理。
		if (m_isJump) {					

			moveSpeed.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity * gravitationalAcceleration;
			//落下しているとき。
			if (moveSpeed.y <= 0) {
				fallTimer += 1;		//滞空時間を加算。
			}
			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity * gravitationalAcceleration) {
				m_isJump = false;
				m_jmpInitialVelocity = 3.f;
				//落下ダメージ。
				TakenDamage(FallDamage());
			}
		}
		else
		{
			//自由落下。
			if (!m_characon.IsOnGround()) {
				m_fallSpeed += 0.1f;
				moveSpeed.y -= m_gravity + m_fallSpeed;		//自由落下。
				fallTimer += 1;		//滞空時間を加算。
			}
			else
			{
				//落下ダメージ。
				TakenDamage(FallDamage());
				m_fallSpeed = 0.5f;
			}
		}
	}
	if (m_characon.IsOnGround()) {
		fallTimer = 0;
	}
}

//落下ダメージ。
int Player::FallDamage()
{
	if (m_gameMode->GetGameMode() != GameMode::enGameModeSurvival) {
		return 0;
	}
	int fallSpeed = fallTimer;			//落下時間。
	const int damageSpeed = 5;			//1ダメージが発生する落下時間。
	int fallDamage = 0;					//落下ダメージ。
	int damageReduction = 5;			//ダメージ軽減。

	if (fallSpeed <= damageSpeed * damageReduction) { return 0; }		//落下時間が30frame以下ならダメージを受けない。

	fallDamage = abs(fallSpeed) / damageSpeed;

	fallTimer = 0;						//タイマーをリセット。

	if (fallDamage <= 10) {
		fallDamage -= damageReduction;		//無敵時間分ダメージを軽減する。
	}
	return fallDamage;
}

//回転処理。
void Player::Turn()
{
	//マウスの移動量を取得。
	CVector2 mouseCursorMovePow = MouseCursor().GetMouseMove() * turnMult * GetDeltaTimeSec();
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
	m_radianY = M_PI / 180.f * m_degreeY;
	m_radianXZ = M_PI / 180.f * m_degreeXZ;

	//回転を計算。
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);
	m_skinModelRender->SetRot(modelRot);
	Headbang();
	Shoulder();
	//右方向と正面方向のベクトルの計算。
	m_right = { -1.0f,0.0f,0.0f };
	m_rotation.Multiply(m_right);
	m_front = { 0.0f,0.0f,-1.0f };
	m_rotation.Multiply(m_front);
}

//しゃがみの処理。
void Player::Shift()
{
	CQuaternion bodyRot;			//体の骨の回転。
	CQuaternion rightLegRot;		//右足の骨の回転。
	CQuaternion leftLegRot;			//左足の骨の回転。
	Bone* bodyBone = m_skinModelRender->FindBone(L"Bone010");		//胴体の骨。
	Bone* rightLegBone = m_skinModelRender->FindBone(L"Bone015");	//右足の骨。
	Bone* leftLegBone = m_skinModelRender->FindBone(L"Bone012");	//左足の骨。
	const float shiftDir = 30.f;			//しゃがむ角度。

	//しゃがみの処理(Boneの回転処理)。GUI表示中は行わない。
	if (GetKeyInput(VK_SHIFT) && m_openedGUI == nullptr) {

		bodyRot.SetRotationDeg(CVector3::AxisZ(), shiftDir);
		rightLegRot.SetRotationDeg(CVector3::AxisX(), shiftDir);
		leftLegRot.SetRotationDeg(CVector3::AxisX(), -shiftDir);

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);

		//キャラコンをしゃがみ移動状態にする
		m_characon.SetIsShiftMove(true);
	}
	//元に戻る処理。
	if (GetKeyUp(VK_SHIFT)) {

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);

		//キャラコンのしゃがみ移動状態を解除
		m_characon.SetIsShiftMove(false);
	}
}

//頭の回転処理。
void Player::Headbang()
{
	m_headBoneRot.SetRotationDeg(CVector3::AxisZ(), m_degreeXZ);
	m_headBone->SetRotationOffset(m_headBoneRot);
}

//攻撃処理。
void Player::Attack()
{
	
	if (GetKeyDown(VK_LBUTTON)) {
		//攻撃判定の座標。
		CVector3 frontAddRot = m_front;			//プレイヤーの向き。
		CQuaternion rot;						//計算用使い捨て。
		rot.SetRotationDeg(m_right, m_degreeXZ);
		rot.Multiply(frontAddRot);

		CVector3 colPos = GetModelPos() + CVector3::Up() * GameCamera::height;
		colPos += frontAddRot * Block::WIDTH;
		//CVector3 colPos = m_gameCamera->GetPos() + frontAddRot * Block::WIDTH;

		//攻撃判定用の当たり判定を作成。
		SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
		attackCol->CreateBox(colPos, m_rotation, Block::WIDTH);
		attackCol->SetTimer(0);		//寿命１フレーム。
		attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
			if (param.EqualName(L"CEnemy")) {			//名前検索。
				Enemy* enemy = param.GetClass<Enemy>();
				enemy->TakenDamage(m_attackPower);
				SuicideObj::CSE* se;
				se = NewGO<SuicideObj::CSE>(m_attackName);
				se->SetVolume(0.25f);
				se->Play();
				m_attackFlag = true;
			}
			if (param.EqualName(L"CAnimals")) {			//名前検索。
				Animals* animals = param.GetClass<Animals>();
				animals->TakenDamage(m_attackPower);
				SuicideObj::CSE* se;
				se = NewGO<SuicideObj::CSE>(m_attackName);
				se->SetVolume(0.25f);
				se->Play();
				m_attackFlag = true;
			}
		});
	}
}

void Player::KnockBack()
{
	if (m_playerState == enPlayerState_KnockBack)
	{
		float knockBackFrame = 25.f;			//ノックバックするフレーム数(60FPS)。


		if (m_knockBackTimer < knockBackFrame) {

			//ノックバック処理。
			CVector3 direction;
			direction = m_knockBackDirection;
			direction.Normalize();
			direction.y = 0.f;
			CVector3 moveSpeed = CVector3::Zero();
			moveSpeed.x += direction.x * m_knockBack * Block::WIDTH;
			moveSpeed.z += direction.z * m_knockBack * Block::WIDTH;

			//高さの処理。
			m_knoceBackY = m_knockBack;
			moveSpeed.y += m_knoceBackY * Block::WIDTH / 2.0f;
			m_knoceBackY -= m_knoceBackY + 0.5f * (1 * m_knockBack) / (knockBackFrame * 2) * (knockBackFrame * 2);	//V0 + 1/2gtt;
			moveSpeed *= 15.0f;

			m_position = m_characon.Execute(moveSpeed);
			m_skinModelRender->SetPos(m_position);

			//モデルの色を赤みがかったようにする。
			m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
				mat->SetAlbedoScale({ CVector4::Red() });
			});
			m_knockBackTimer += 1;		//タイマーを加算。
		}
		else {		//リセット。
			m_playerState = enPlayerState_idle;
			m_knockBackTimer = 0.f;
			m_knoceBackY = 1.f;
		}

	}
	else 
	{		
		//モデルの色を元に戻す。
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
		mat->SetAlbedoScale({ CVector4::White() });
		});
	}
}

//インベントリを開く。
void Player::OpenInventory()
{
	//Eボタンを押したとき。
	if (GetKeyDown('E')){
		OpenGUI( std::make_unique<GUI::PlayerInventory>( m_inventory ) );
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
		m_skinModelRender->GetAnimCon().SetSpeed(0.9f);
		m_runSpeedDouble = 1.f;
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			m_stamina -= 0.0003f;
		}

		break;
	case Player::enPlayerState_run:		//走っているとき。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.2f);
		m_runSpeedDouble = 2.f;
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			m_stamina -= 0.003f;
		}

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

//オブジェクトの設置と破壊。
void Player::InstallAndDestruct(const Block* hitBlock, const CVector3& hitnormal)
{
	SuicideObj::CSE* se;
	se = NewGO<SuicideObj::CSE>(m_putName);
	se->SetVolume(0.5f);

	//設置。
	if (GetKeyDown(VK_RBUTTON)) {
		CVector3 installPos;		//設置する場所。
		installPos = hitBlock->CalcBlockUnitPos().CastToCVector3();

		//当たり判定がブロックでないとき(ゾンビとか)。
		if(m_world->GetBlock(installPos) == nullptr){
			return;
		}
		//ブロックに設定された右クリック時のアクションを実行する。(作業台を開くだとか、そんなもの)
		bool isClickActionDone = m_world->GetBlock( installPos )->OnClick( this );
		//アクションが実行されなかった場合だけ、通常通りブロックの設置を行う。
		if( isClickActionDone == false ){

			auto& item = m_inventory.GetItem(m_selItemNum - 1);		//アイテムの参照。
			if (item != nullptr) {
				if (item->GetIsBlock()) {		//ブロック。
					se->Play();
					installPos += hitnormal;

					Block::enMuki muki = CalcMukiReverse( GetFront() );

					if (m_world->PlaceBlock(installPos, BlockFactory::CreateBlock(static_cast<EnCube>(item->GetID()), muki))) {
						//設置に成功したらインベントリのブロック数減らす
						auto item = m_inventory.TakeItem(m_selItemNum - 1, 1);
					}
				}
			}
		}
	}
	//破壊。ここもInputに変えた。
	if (GetKeyInput(VK_LBUTTON) && !m_attackFlag) {
		auto& item = m_inventory.GetItem(m_selItemNum - 1);
		const Block* block = nullptr;
		
		//破壊。
		//アイテムを持っているかで分岐
		if (item) {
			block = m_world->DamegeBlock(hitBlock->CalcBlockUnitPos().CastToCVector3(), (EnTool)item->GetToolID(), item->GetToolLevel());
		}
		else {
			block = m_world->DamegeBlock(hitBlock->CalcBlockUnitPos().CastToCVector3());
		}

		//ひび割れモデル表示
		if (block) {
			m_blockCrackModel.SetIsDraw(true);
			m_blockCrackModel.SetPos(block->GetModelPos());
			m_blockCrackModel.RefreshWorldMatrix();//モーションブラーが出ないように更新
			m_blockCrackModel.GetSkinModel().FindMaterialSetting(
				[&](MaterialSetting* mat) {
					mat->SetUVOffset({-0.1f*((int)(block->GetHP_Ratio()*10.0f)),0.0f});//UVアニメーション				
				}
			);

			//SE
			SuicideObj::CSE* se;
			se = NewGO<SuicideObj::CSE>(m_strikeName);
			se->SetVolume(0.25f);
			if (isStrikeFlag){
				se->Play();
				isStrikeFlag = false;
			}
			else if (!se->GetIsPlaying()){
				isStrikeFlag = true;
			}
		}
		else {
			m_blockCrackModel.SetIsDraw(false);
		}
	}
	m_attackFlag = false;
}

//ブロック破壊を実行するかどうか判断する。
void Player::DecideCanDestroyBlock()
{
	//マウス左長押しなら。
	if (GetKeyInput(VK_LBUTTON) || GetKeyDown(VK_LBUTTON)){
		//タイマーを+する。
		m_isBlockDestruction = true;
		m_timerBlockDestruction += GetDeltaTimeSec();
		//タイマーが一定時間以下なら破壊を実行しない。
		if (m_gameMode->GetGameMode() == GameMode::enGameModeCreative){
			m_isBlockDestruction = true;
		}
		else if (m_timerBlockDestruction <= timeBlockDestruction){
			m_isBlockDestruction = false;
		}
		//タイマーが一定時間以上ならタイマーをリセットし、レイを飛ばす。
		else {
			m_isBlockDestruction = true;
			m_timerBlockDestruction = 0.0f;
		}
	}
	//マウス長押しされてない場合、マウスを押した瞬間に破壊できるようにタイマーをセットしておく。
	else {
		m_isBlockDestruction = false;
		m_timerBlockDestruction = timeBlockDestruction;

		//ブロック破壊モデル非表示
		m_blockCrackModel.SetIsDraw(false);
	}
}

//レイを前方に飛ばす。
void Player::FlyTheRay()
{	
	//マウス左長押しなら。				
	if (GetKeyDown(VK_RBUTTON) || GetKeyInput(VK_LBUTTON) || GetKeyDown(VK_LBUTTON)) {
		//マウス左長押しかつ破壊フラグがたっていなかったら、処理しない。
		if (GetKeyInput(VK_LBUTTON) && !m_isBlockDestruction){
			return;
		}
		const int up = 75;
		upDownY = up;
		int reyLength = installableBlockNum * Block::WIDTH;		//レイの長さ。		 
		CVector3 frontAddRot = m_front;			//プレイヤーの向き。
		CQuaternion rot;						//計算用使い捨て。
		rot.SetRotationDeg(m_right, m_degreeXZ);
		rot.Multiply(frontAddRot);

		//雑判定
		CVector3 returnHitNormal;
		CVector3 sampPos = GetModelPos() + CVector3::Up() * GameCamera::height;
		Block* block = m_world->RayTestBlock(sampPos, sampPos + frontAddRot * reyLength, nullptr, &returnHitNormal);
		if (block) {
			InstallAndDestruct(block, returnHitNormal);
			return;
		}
		m_blockCrackModel.SetIsDraw(false);

		/*
		btVector3 startPoint(m_gameCamera->GetPos());					//レイの視点。
		btVector3 endPoint(startPoint + frontAddRot * reyLength);		//レイの終点。

		ClosestRayResultCallbackForCCollisionObj rayRC(startPoint, endPoint, L"Block");		//レイ情報(ブロックとのみ判定)。
		GetEngine().GetPhysicsWorld().GetDynamicWorld()->rayTest(startPoint, endPoint, rayRC);		//レイを飛ばす。
		if (rayRC.hasHit()) {		//衝突。
			InstallAndDestruct(rayRC , frontAddRot);
		}
		else {
			m_blockCrackModel.SetIsDraw(false);
		}
		*/
	}
}

//被ダメ－ジ。
void Player::TakenDamage(int AttackPow, CVector3 knockBackDirection, bool isAttacked, bool ignoreDefence)
{
	if (m_hp > 0 && AttackPow > 0) {			//被弾する。

		float damage = AttackPow;		//被ダメ。

		if (!ignoreDefence) {
			//防御力の計算。
			damage = AttackPow * (1.f - m_defensePower * 0.04f);
		}
		m_hp -= damage;

		//HPを0未満にしない。
		if (m_hp < 1) {			
			m_hp = 0;
		}

		//死んでないときのみ実行
		if (m_hp > 0) {
			//カメラ回転		
			m_gameCamera->SetRollDeg(CMath::RandomZeroToOne() > 0.5f ? 25.0f : -25.0f);
			
			//ダメージボイス
			SuicideObj::CSE* voice;
			//２種類からランダムで音が鳴る。
			if (CMath::RandomZeroToOne() > 0.5f) {
				voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/damage.wav"); 
				voice->SetVolume(0.25f);
			}
			else {
				voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/damage.wav");
				voice->SetVolume(0.25f);
			}
			voice->Play();
			//攻撃されたのなら、ノックバックする。
			if (isAttacked) {
				m_playerState = enPlayerState_KnockBack;
				m_knockBackDirection = knockBackDirection;
				m_knockBackDirection.Normalize();
			}
		}
	}
}

//死亡処理。
void Player::Death()
{
	//死亡状態かの判定。
	if (m_hp <= 0) {
		m_playerState = enPlayerState_death;
		m_deathFlag = true;
	}
	//死亡した時。
	if (m_playerState == enPlayerState_death) {
		float maxRot = 90.f;							//回転の上限値。
		float rotEndTime = 0.5f;						//回転終了までにかかる時間。 
		float oneFrameRot = maxRot / 60.f / rotEndTime;			//1フレームの回転量。		

		//プレイヤーの回転処理。
		if (m_deathAddRot <= maxRot) {	//回転量が上限に達するまで。
			CQuaternion deathRot;		//死亡時に加算する回転量。
			deathRot.SetRotationDeg(CVector3::AxisZ(), oneFrameRot);
			m_rotation.Multiply(deathRot);
			m_skinModelRender->SetRot(m_rotation);
			m_deathAddRot += oneFrameRot;
		}
		//モデルの色を赤みがかったようにする。
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetAlbedoScale({ CVector4::Red() });
		});
		//死亡時のUI。
		if (m_playerDeath == nullptr) {
			m_playerDeath = NewGO<PlayerDeath>();
			m_playerDeath->SetExp(m_exp);

			//死亡中一度だけ実行
			{
				//ダメージエフェクト
				NewGO<DamegeScreenEffect>();
				//首折れる
				m_gameCamera->SetRollDeg(CMath::RandomZeroToOne() > 0.5f ? 90.0f : -90.0f, true);
				//ダメージボイス
				SuicideObj::CSE* voice;
				voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/damage.wav");
				voice->Play();

				for (int i = 0; i < 40; i++) {
					auto item = m_inventory.TakeAllItem(i);
					if (item) {
						CVector3 pos = GetPos() + GetFront() * Block::WIDTH;
						pos.y += Block::WIDTH;
						DropItem* drop = DropItem::CreateDropItem(m_world, std::move(item));
						CVector3 addPos = CVector3::Zero();
						if (random() % 2 > 0) {
							addPos.x += rand() % randomDrop;
						}
						else {
							addPos.x -= rand() % randomDrop;
						}

						if (random() % 2 > 0) {
							addPos.z += rand() % randomDrop;
						}
						else {
							addPos.z += rand() % randomDrop;
						}
						drop->SetPos(pos+addPos);
					}
				}			
			}
		}
		//リスポーン。
		if (m_playerDeath->Click() == m_playerDeath->enButtonResupawn) {
			Respawn();
		}
		//タイトルへ戻る。
		else if (m_playerDeath->Click() == m_playerDeath->enButtonRerturnToTitle) {
			m_game->TransToTitle();
		}
	}
}

//リスポーン。
void Player::Respawn()
{
	m_deathAddRot = 0.f;					//プレイヤーの回転量の初期化。。
	m_hp = 20.f;								//HPの初期化。
	m_stamina = 20.f;
	m_playerState = enPlayerState_idle;		//プレイヤーの状態の初期化。
	m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
		mat->SetAlbedoScale({ CVector4::White() });		//モデルの色の初期化。
	});	
	m_gameCamera->SetRollDeg(0.0f);			//首蘇生
	m_characon.SetPosition(m_respawnPos);
	DeleteGO(m_playerDeath);
	CloseGUI();
}

//モデルの描画をするか。
void Player::IsDraw()
{
	if (m_gameCamera->GetCameraMode() == EnMode_FPS) {
		m_skinModelRender->SetIsDraw(false);
		m_playerArmor->IsDraw(false);
	}
	else{
		m_skinModelRender->SetIsDraw(true);
		m_playerArmor->IsDraw(true);
	}
}

void Player::Stamina()
{
	if (m_stamina <= 0) {
		m_stamina = 0;
	}
	if (m_stamina >= 21) {
		m_stamina = 21;
	}

	const float maxTimer = 1.5f;
	//飯を食べる処理。
	if (GetKeyInput(VK_RBUTTON)) {
		auto& item = m_inventory.GetItem(m_selItemNum - 1);		//アイテムの参照。
		if ((item == nullptr || item->IsFood() == false) && !GetKeyInput(VK_TAB))//食べ物かの判別。または強制食事モード
		{
			return;
		}
		else{
			m_eatingTimer += GetDeltaTimeSec();		//タイマー回すよん。
			m_eatingFlag = true;
			if (m_eatingTimer >= maxTimer)
			{
				m_eatingTimer = 0.0f;
				hiddenStamina = 4;			//隠れスタミナを上昇する。
				if (item) {//アイテム食べる
					m_stamina += item->GetFoodLevel();//スタミナ回復
					auto item = m_inventory.TakeItem(m_selItemNum - 1, 1);	//アイテムの数を減らす。
				}
				else {//腕食べる
					m_stamina += 3.0f;//スタミナ回復
					TakenDamage(8,CVector3::Up(),false,true);
				}
			}
		}
	}
	else
	{
		m_eatingFlag = false;
		m_eatingTimer = 0.0f;
	}
	//HP回復処理。
	if (hiddenStamina > 0 && m_stamina >= 20) {
		staminaTimer += 1;
		if(staminaTimer >= 30){
			if (m_hp < 20) {
				m_hp += 1;
			}
			hiddenStamina -= 1;
			staminaTimer = 0;
		}
	}
}

//空腹時のダメージ。
void Player::HungryDamage()
{
	if (m_gameMode->GetGameMode() != GameMode::enGameModeSurvival) {		//クリエイティブのとき処理をしない。
		return;
	}
	if (m_stamina <= 0 && m_hp > 0) {		//スタミナが０のとき。
		hungryDamageTimer++;
		if (hungryDamageTimer >= 60) {
			TakenDamage(1, CVector3::Zero(), false, true);
			hungryDamageTimer = 0;
		}
	}
	else{
		hungryDamageTimer = 0;
	}
}

//アイテムを投げる処理。
void Player::ThrowItem()
{
	if (GetKeyDown('Q')) {
		auto item = m_inventory.TakeItem(m_selItemNum - 1, 1);
		if (item) {
			CVector3 pos = GetPos() + GetFront() * Block::WIDTH;
			pos.y += Block::WIDTH;
			DropItem* drop = DropItem::CreateDropItem(m_world, std::move(item));
			drop->SetPos(pos);
			drop->SetVelocity(GetFront() * 300);
		}
	}
}

//肩
void Player::Shoulder()
{
	const int Down = 5;
	m_shoulderBoneRot.SetRotationDeg(CVector3::AxisX(), upDownY);
	m_shoulderBone->SetRotationOffset(m_shoulderBoneRot);
	if (upDownY > 0){
		upDownY -= Down;
	}
	else{
		upDownY = 0;
	}
}

void Player::Defence()
{
	m_defensePower = 0;
	if (m_inventory.GetNullableItem(36).GetID() != enCube_None)
	{
		m_defensePower += m_inventory.GetItem(36).get()->GetToolLevel();
	}
	if (m_inventory.GetNullableItem(37).GetID() != enCube_None)
	{
		m_defensePower += m_inventory.GetItem(37).get()->GetToolLevel();
	}
	if (m_inventory.GetNullableItem(38).GetID() != enCube_None)
	{
		m_defensePower += m_inventory.GetItem(38).get()->GetToolLevel();
	}
	if (m_inventory.GetNullableItem(39).GetID() != enCube_None)
	{
		m_defensePower += m_inventory.GetItem(39).get()->GetToolLevel();
	}
}

void Player::CalcAttackPow() {
	m_attackPower = HAND_ATTACK_POW;
	auto& item = m_inventory.GetItem(m_selItemNum - 1);
	if (item) {
		if (item->GetToolID() == enTool_Sword) {
			m_attackPower *= item->GetToolLevel();
		}
		if (item->GetToolID() == enTool_Axe) {
			m_attackPower *= max(1, item->GetToolLevel() / 2);
		}
	}
}

void Player::HUDRender(int HUDNum)  {
	CVector3 pos = GetPos() / Block::WIDTH;
	IntVector3 sampPos = { (int)std::floor(pos.x),(int)std::floor(pos.y + 0.5f),(int)std::floor(pos.z) };
	char* light = m_world->GetLightData(sampPos);
	char* skylight = m_world->GetSkyLightData(sampPos);
	Block* block = m_world->GetBlock(sampPos);

	//座標表示
	std::wstringstream str;
	str << pos.x << " , " << pos.y << " , " << pos.z << "\n";
	//ライト表示
	if (light && skylight) {
		str << "blockLight:" << (int)*light << " skyLight:" << (int)*skylight << "\n";
	}
	//ブロックID
	if (block) {
		str << "blockID:" << block->GetBlockType() << "\n";
	}
	font.Draw(str.str().c_str(), { 0.9f , 0.1f }, CVector4::White(), 0.5f, { 0.5f, 0.5f });
}

void Player::CreateFrontDropItem(std::unique_ptr<ItemStack>& item)
{

	CVector3 pos = GetPos() + GetFront() * Block::WIDTH;
	pos.y += Block::WIDTH;
	DropItem* drop = DropItem::CreateDropItem(m_world, item->GetID(),item->GetNumber());
	drop->SetPos(pos);
	drop->SetVelocity(GetFront() * 300);

}