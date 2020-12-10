#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "Item.h"
#include "GameMode.h"
#include "World.h"
#include "InventoryGUI.h"
#include "BlockFactory.h"
#include "ItemDisplay.h"

namespace {
	const float turnMult = 20.0f;			//プレイヤーの回転速度。
	const float maxDegreeXZ = 70.0f;		//XZ軸の回転の最大値。
	const float minDegreeXZ = -50.0f;		//XZ軸の回転の最小値。
	const float moveMult = 8.0f;			//プレイヤーの移動速度。
	const float move = 1.0f;				//移動速度(基本的には触らない)。
	const float gravitationalAcceleration = 0.3f;		//todo これ多分いらんわ 重力加速度。
	const float doubleClickRug = 0.2f;		//ダブルクリック判定になる間合い。

	CVector3 stickL = CVector3::Zero();		//WSADキーによる移動量
	CVector3 moveSpeed = CVector3::Zero();		//プレイヤーの移動速度(方向もち)。
	CVector3 itemDisplayPos = CVector3::Zero();	//アイテム（右手部分）の位置。
}

Player::Player() : m_inventory(36)
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
}

bool Player::Start()
{
	//プレイヤークラスの初期化。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/player.tkm", m_animationClip, enAnimationClip_Num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetRot(m_rotation);

	//キャラコンの初期化。
	m_characon.Init(m_characonRadius, m_characonHeight, m_position);

	//被弾判定用コリジョン。
	m_damageCollision = std::make_unique<SuicideObj::CCollisionObj>();
	CVector3 colPos = (m_position.x, m_position.y + Block::WIDTH, m_position.z);		//コリジョン座標。
	m_damageCollision->CreateCapsule(colPos, m_rotation, m_characonRadius, m_characonHeight);
	m_damageCollision->SetTimer(enNoTimer);				//寿命無し。
	m_damageCollision->SetName(L"CPlayer");
	m_damageCollision->SetClass(this);					//クラスのポインタを取得。
	m_damageCollision->SetIsHurtCollision(true);		//自分から判定をとらない。

	//インベントリ―の初期化。
	for (int i = 0; i < inventryWidth; i++) {
		//m_inventoryList[i] = new Inventory();
		//m_inventoryList[i]->s_item = GetItemData().GetItem(enCube_None);
	}

	//右手表示のclassにゅうごー
	m_rightHandDisplay = NewGO<ItemDisplay>();
	m_rightHandDisplay->SetName(L"ItemDisplay");
	m_rightHandDisplay->SetPos(m_position);
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
	//todo Debug専用。
	if( GetKeyDown( 'C' ) ){			//マウスカーソルを固定(解除)。
		static bool lock = true;
		MouseCursor().SetLockMouseCursor( lock = !lock );
	}

	//頭の骨を取得。
	m_headBone = m_skinModelRender->FindBone(L"Bone002");

	//移動処理。
	Move();
	//回転処理。
	Turn();
	//インベントリを開く。
	OpenInventory();
	//プレイヤーの状態管理。
	StateManagement();
	//前方にRayを飛ばす。
	FlyTheRay();

	Test();
	//右手の更新処理。
	ItemDisplayUpdate();
}

void Player::SetWorld(World* world, bool recursive) {
	m_world = world;
	if (recursive)
		world->SetPlayer(this, false);
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

//走る処理。
void Player::Dash()
{
	//Wダブルクリック。
	if (GetKeyUp('W')) {
		m_doubleCilckFlag = true;
	}
	if (m_doubleCilckFlag) {
		if (m_doubleClickTimer <= doubleClickRug && m_playerState != enPlayerState_run) {
			m_doubleClickTimer += GetEngine().GetRealDeltaTimeSec();	//タイマーを進める。
		}
		if (m_doubleClickTimer <= doubleClickRug) {
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

	//Ctrl+W。
	if (GetKeyInput('W') && GetKeyInput(VK_CONTROL)) {
		m_playerState = enPlayerState_run;
	}
	if (m_playerState == enPlayerState_run) {
		if (GetKeyUp('W') || GetKeyUp(VK_CONTROL)) {
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
	m_skinModelRender->SetPos(m_position);
	//右手も移動させる。
	m_rightHandDisplay->SetPos(m_position);
	//ダメージ当たり判定移動。
	CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//当たり判定の座標。
	m_damageCollision->SetPosition(colPos);

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
	if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival		//サバイバルのときか。
		|| m_flyingMode == false) {										//クリエイティブのフライモードでないとき。
		if (GetKeyInput(VK_SPACE) && m_characon.IsOnGround()) {			//スペースが押されていたら&&地面にいたら。
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
	m_radianY = M_PI / 180 * m_degreeY;
	m_radianXZ = M_PI / 180 * m_degreeXZ;

	//回転を計算。
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CQuaternion modelRot;
	modelRot.SetRotationDeg(CVector3::AxisY(), m_degreeY + 180.0f);
	m_skinModelRender->SetRot(modelRot);
	Headbang();

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

	//しゃがみの処理(Boneの回転処理)。
	if (GetKeyInput(VK_SHIFT)) {
		//todo ブロックから落ちない処理を追加する。
		bodyRot.SetRotationDeg(CVector3::AxisZ(), shiftDir);
		rightLegRot.SetRotationDeg(CVector3::AxisX(), shiftDir);
		leftLegRot.SetRotationDeg(CVector3::AxisX(), -shiftDir);

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);
	}
	//元に戻る処理。
	if (GetKeyUp(VK_SHIFT)) {
		bodyRot.SetRotationDeg(CVector3::AxisZ(), -shiftDir* 0.5f);
		rightLegRot.SetRotationDeg(CVector3::AxisX(), -shiftDir* 0.5f);
		leftLegRot.SetRotationDeg(CVector3::AxisX(), shiftDir* 0.5f);

		bodyBone->SetRotationOffset(bodyRot);
		rightLegBone->SetRotationOffset(rightLegRot);
		leftLegBone->SetRotationOffset(leftLegRot);
	}
}

//頭の回転処理。
void Player::Headbang()
{
	m_headBoneRot.SetRotationDeg(CVector3::AxisZ(), m_degreeXZ);
	m_headBone->SetRotationOffset(m_headBoneRot);
}

//インベントリを開く。
void Player::OpenInventory()
{
	if (GetKeyDown('E')){		//Eボタンを押したとき。
		//インベントリを開く。
		if (!m_inventoryGUI) {
			m_inventoryGUI = std::make_unique<GUI::InventoryGUI>(m_inventory);
			MouseCursor().SetLockMouseCursor(false);		//マウスカーソルの固定を外す。
		}else{
		//インベントリを閉じる。
			m_inventoryGUI.reset();
			MouseCursor().SetLockMouseCursor(true);		//マウスカーソルを固定する。
		}
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

		break;
	case Player::enPlayerState_run:		//走っているとき。

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, 0.3f);
		m_skinModelRender->GetAnimCon().SetSpeed(1.2f);
		m_runSpeedDouble = 2.f;

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
void Player::InstallAndDestruct(btCollisionWorld::ClosestRayResultCallback ray, CVector3 frontRotAdd)
{
	frontRotAdd.Normalize();

	//設置。
	if (GetKeyDown(VK_RBUTTON)) {
		CVector3 installPos;
		installPos = (ray.m_hitPointWorld - frontRotAdd) / Block::WIDTH;
		m_world->PlaceBlock(installPos, BlockFactory::CreateBlock(enCube_CraftingTable));
	}
	//破壊。
	if (GetKeyDown(VK_LBUTTON)) {
//		m_world->GetBlock((ray.m_hitPointWorld + frontRotAdd) / Block::WIDTH)->GetBlockType();		//ブロックの種類を取得。
		m_world->DeleteBlock((ray.m_hitPointWorld + frontRotAdd) / Block::WIDTH) ;					//破壊。
	}
}

//レイを前方に飛ばす。
void Player::FlyTheRay()
{
	if (GetKeyDown(VK_RBUTTON) || GetKeyDown(VK_LBUTTON)) {
		const int installableBlockNum = 5;		//設置可能距離(ブロック距離)。
		int reyLength = installableBlockNum * Block::WIDTH;		//レイの長さ。		 
		CVector3 frontAddRot = m_front;			//プレイヤーの向き。
		CQuaternion rot;						//計算用使い捨て。
		rot.SetRotationDeg(m_right, m_degreeXZ);
		rot.Multiply(frontAddRot);

		btVector3 startPoint(m_gameCamera->GetPos());					//レイの視点。
		btVector3 endPoint(startPoint + frontAddRot * reyLength);		//レイの終点。
		//todo Debug Ray描画用。
		CVector3 kariX = m_gameCamera->GetPos() + GetMainCamera()->GetFront() * 100;
		CVector3 kariY = kariX + frontAddRot * reyLength;
		DrawLine3D(kariX, kariY, CVector4::Green());

		btCollisionWorld::ClosestRayResultCallback rayRC(startPoint, endPoint);		//レイ情報。
		GetEngine().GetPhysicsWorld().GetDynamicWorld()->rayTest(startPoint, endPoint, rayRC);		//レイを飛ばす。
		if (rayRC.hasHit()) {		//衝突。
			InstallAndDestruct(rayRC , frontAddRot);
		}
	}
}

//被ダメ－ジ。
void Player::TakenDamage(int AttackPow)
{
	if (m_hp > 0) {			//被弾する。
		m_hp -= AttackPow;
	}
	if(m_hp <= 0){			//HPを0未満にしない。
		m_hp = 0;
	}
}

//todo Debug専用。
void Player::Test()
{
	if (GetKeyUp(VK_LEFT) && m_hp > 0) {		//体力減少。
		m_hp -= 1;
		if (m_defensePower > 0) {
			m_defensePower -= 1;				//防御力減少。
		}
	}
	if (GetKeyUp(VK_RIGHT) && m_hp < 20) {		//体力上昇。
		m_hp += 1;
		if (m_defensePower < 20) {
			m_defensePower += 1;				//防御力上昇。
		}
	}
	if (GetKeyUp(VK_UP) && m_stamina < 20) {	//スタミナ上昇。
		m_stamina += 1;
	}
	if (GetKeyUp(VK_DOWN) && m_stamina > 0) {	//スタミナ減少。
		m_stamina -= 1;
	}
	if (GetKeyUp(VK_NUMPAD1) && m_exp > 0) {	//経験値減少。
		m_exp -= 0.3f;			
	}
	if (GetKeyUp(VK_NUMPAD2)) {					//経験値増加。
		m_exp += 0.3f;
	}
}

//右手表示の更新処理。
void Player::ItemDisplayUpdate()
{
	//右手に位置と回転を送ってます。
	//m_rightHandDisplay->SetPos(m_position);
	//m_rightHandDisplay->SetRot(m_rotation);
}