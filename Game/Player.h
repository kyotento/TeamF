#pragma once
#include "Block.h"
#include "Entity.h"
#include "Inventory.h"
#include "MCCharaCon.h"

class World;
class GameCamera;
class Item;
class GameMode;
class ItemDisplay;
class PlayerParameter;
class PlayerDeath;
class PlayerArmor;
class Game;
class ItemStack;
namespace GUI{
	class RootNode;
}

class Player : public Entity
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;

	CFont font;
	void HUDRender(int HUDNum) override;

	/// <summary>
	/// アニメーション。
	/// </summary>
	enum enAnimationClip{
		enAnimationClip_Idle,			//待機状態。
		enAnimationClip_move,			//移動。
		enAnimationClip_excavate,		//物を掘る。
		enAnimationClip_Num,			//アニメーションの数。
	};

	AnimationClip m_animationClip[enAnimationClip_Num];			//アニメーションの数を格納。

	/// <summary>
	/// プレイヤーの状態。
	/// </summary>
	enum enPlayerState {
		enPlayerState_idle,				//待機。	
		enPlayerState_move,				//移動。
		enPlayerState_run,				//走っているとき。
		enPlayerState_excavate,			//物を掘る。
		enPlayerState_KnockBack,		//ノックバック。
		enPlayerState_death,			//死んだとき。
		enPlayerState_sleep,			//睡眠中。
		enPlayerState_num,				//状態の数。
	};

	/// <summary>
	/// プレイヤーの状態を取得。
	/// </summary>
	/// <returns>プレイヤーの状態</returns>
	enPlayerState GetplayerState()const {
		return m_playerState;
	}

	/// <summary>
	/// プレイヤーがしゃがんでいるか取得。
	/// </summary>
	/// <returns></returns>
	bool GetIsSneaking()const {
		return m_characon.IsShiftMove();
	}

	/// <summary>
	/// プレイヤーの右方向を取得。
	/// </summary>
	/// <returns>プレイヤーの右方向</returns>
	const CVector3& GetRight()
	{
		return m_right;
	}

	/// <summary>
	/// プレイヤーの正面方向を取得。
	/// </summary>
	/// <returns>プレイヤーの正面方向</returns>
	const CVector3& GetFront()
	{
		return m_front;
	}
	
	//! @brief 座標を取得。
	CVector3 GetPos() const override{
		return m_position;
	}

	//! @brief モデル座標を取得。
	const CVector3& GetModelPos() const {
		if (!m_skinModelRender) {
			return CVector3::Zero();
		}
		return m_skinModelRender->GetPos();
	}
	/// <summary>
	/// 右手の座標を取得。
	/// </summary>
	/// <returns>右手のボーン座標。</returns>
	const CVector3& GetRightHandPos()  const
	{
		if (!m_rightHandBone)
		{
			return CVector3::Zero();
		}
		return m_rightHandBone->GetPosition();
	}
	//! @brief 回転を取得。
	CQuaternion GetRot() const {
		return m_rotation;
	}

	//! @brief 座標を設定。
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
	}

	//キャラクターのAABBを取得
	const AABB& GetAABB()const {
		return m_characon.GetAABB();
	}

	/// <summary>
	/// エンティティの有効切替時の処理
	/// </summary>
	/// <param name="enable"></param>
	void SetEnableEntityInner(bool enable)override {
		//プレイヤーは無効化されないでしょ
	}

	/// <summary>
	/// リスポーン地点の変更
	/// </summary>
	/// <param name="pos">新しいリスポーン地点の座標</param>
	void SetRespawnPos(const CVector3& pos) {
		m_respawnPos = pos;
	}
	/// <summary>
	/// リスポーン地点の取得
	/// </summary>
	const CVector3& GetRespawnPos() const{
		return m_respawnPos;
	}

	/// <summary>
	/// HPを設定する。
	/// </summary>
	/// <param name="stamina">設定するHP</param>
	void SetHP( float hp ){
		m_hp = hp;
	}

	/// <summary>
	/// HPを取得する。
	/// </summary>
	/// <returns>HP</returns>
	float GetHP() const
	{
		return m_hp;
	}

	/// <summary>
	/// スタミナを設定する。
	/// </summary>
	/// <param name="stamina">設定するスタミナ</param>
	void SetStamina(float stamina){
		m_stamina = stamina;
	}

	/// <summary>
	/// スタミナを取得する。
	/// </summary>
	/// <return>スタミナ</return>
	float GetStamina() const
	{
		return m_stamina;
	}

	/// <summary>
	/// 防御力を取得。
	/// </summary>
	/// <returns>防御力</returns>
	int GetDefPow()
	{
		return m_defensePower;
	}

	/// <summary>
	/// 経験値を加算する。
	/// </summary>
	/// <param name="exp">加算する経験値量</param>
	void AddExp(const float exp)
	{
		m_isExpUpFlag = true;
		m_exp += exp;
	}

	/// <summary>
	/// 経験値を設定する。
	/// </summary>
	/// <param name="exp">設定する経験値量</param>
	void SetExp( float exp ){
		m_exp = exp;
	}

	/// <summary>
	/// 経験値を取得する。
	/// </summary>
	/// <returns>経験値</returns>
	float GetExp() const
	{
		return m_exp;
	}

	/// <summary>
	/// 選択するアイテムを指定。
	/// </summary>
	/// <param name="selItem">選択したアイテム番号</param>
	void SetSelectItemNum(int selItem)
	{
		m_selItemNum = selItem;
	}

	/// <summary>
	/// 選択されているアイテム番号を取得。
	/// </summary>
	/// <returns>選択中のアイテム番号</returns>
	int GetSelectItemNum()
	{
		return m_selItemNum;
	}

	/// <summary>
	/// プレイヤーのY軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのY軸の回転</returns>
	float GetRadianY() const
	{
		return m_radianY;
	}

	/// <summary>
	/// プレイヤーのXZ軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのXZ軸の回転</returns>
	float GetRadianXZ() const
	{
		return m_radianXZ;
	}

	/// <summary>
	/// プレイヤーのY軸の回転を設定。
	/// </summary>
	/// <returns>プレイヤーのY軸の回転</returns>
	void SetRadianY( float rot );

	/// <summary>
	/// プレイヤーのXZ軸の回転を設定。
	/// </summary>
	/// <returns>プレイヤーのXZ軸の回転</returns>
	void SetRadianXZ( float rot );

	World* GetWorld(){
		return m_world;
	}

	//インベントリの長さ
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;

	Inventory& GetInventory(){
		return m_inventory;
	}
	const Inventory& GetInventory() const{
		return m_inventory;
	}

	/// <summary>
	/// GUIを開かせる。
	/// </summary>
	/// <param name="gui">開かせるGUI</param>
	void OpenGUI( std::unique_ptr<GUI::RootNode>&& gui );

	/// <summary>
	/// GUIを閉じさせる。
	/// </summary>
	void CloseGUI();


	/// <summary>
	/// 被ダメージ。
	/// </summary>
	/// <param name="AttackePow">攻撃力</param>
	/// <param name="knockBackDirection">ノックバックベクトル</param>
	/// <param name="isAttacked"></param>
	/// <param name="ignoreDefence">防御力を無視するか</param>
	void TakenDamage(int AttackePow, CVector3 knockBackDirection = CVector3::Zero(),bool isAttacked = false, bool ignoreDefence = false);

	/// <summary>
	/// ゲームのインスタンスを設定する。
	/// </summary>
	/// <param name="game"></param>
	void SetGameIns(Game* game)
	{
		m_game = game;
	}
	/// <summary>
	/// プレイヤー死んでる？？
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsPlayerDead()
	{
		return m_deathFlag;
	}
	/// <summary>
	/// ブロック破壊をした？
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsBlockDestruction()
	{
		return m_isBlockDestruction;
	}
	/// <summary>
	/// プレイヤーが死んでるかどうか取得。
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsDeath()
	{
		return m_playerState == enPlayerState_death;
	}
	/// <summary>
	/// プレイヤーが寝ているか取得。
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsSleep()
	{
		return m_playerState == enPlayerState_sleep;
	}
	/// <summary>
	/// 食べてるか。
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsEating()
	{
		return m_eatingFlag;
	}

	/// <summary>
	/// 睡眠状態に切り替え
	/// </summary>
	void SwitchSleep()
	{
		if (m_playerState == enPlayerState_sleep) {
			m_playerState = enPlayerState_idle;
		}
		else {
			m_playerState = enPlayerState_sleep;
		}
	}

	//プレイヤーの前方にドロップアイテムを生成する。
	void CreateFrontDropItem(std::unique_ptr<ItemStack>& item);

	/// <summary>
	/// プレイヤー回転速度取得
	/// </summary>
	/// <returns></returns>
	float GetTurnMult()const {
		return m_turnMult;
	}
	void SetTurnMult(float f) {
		m_turnMult = f;
	}

	/// <summary>
	/// マウスXZ回転反転設定取得
	/// </summary>
	/// <returns></returns>
	bool GetReverseTurnXZ()const {
		return m_reverseTurnXZ;
	}
	void SetReverseTurnXZ(bool f) {
		m_reverseTurnXZ = f;
	}

private:
	/// <summary>
	/// キーボードの入力情報管理。
	/// </summary>
	void KeyBoardInput();

	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();

	/// <summary>
	/// 走る処理。
	/// </summary>
	void Dash();

	/// <summary>
	/// ジャンプ処理。
	/// </summary>
	void Jump();

	/// <summary>
	/// 回転処理。
	/// </summary>
	void Turn();

	/// <summary>
	/// しゃがみの処理。
	/// </summary>
	void Shift();

	/// <summary>
	/// 頭の回転処理。
	/// </summary>
	void Headbang();

	/// <summary>
	/// 攻撃処理。
	/// </summary>
	void Attack();
	/// <summary>
	/// ノックバック
	/// </summary>
	void KnockBack();

	/// <summary>
	/// インベントリを開く。
	/// </summary>
	void OpenInventory();

	/// <summary>
	/// 移動方法の切り替え(クリエイティブ)。
	/// </summary>
	void ChangeMovemontC();

	/// <summary>
	/// プレイヤーの状態管理。
	/// </summary>
	void StateManagement();

	/// <summary>
	/// プレイヤーの設置と破壊。
	/// </summary>
	/// <param name="hitBlock">接触したブロック</param>
	/// <param name="hitnormal">法線</param>
	void InstallAndDestruct(const Block* hitBlock, const CVector3& hitnormal);

	/// <summary>
	/// ブロックを破壊するかどうか判断する。
	/// </summary>
	void DecideCanDestroyBlock();

	// プレイヤーの前方にレイを飛ばす。
	void FlyTheRay();

	// 死亡処理。
	void Death();

	// リスポーン。
	void Respawn();

	// モデルの描画をするか。
	void IsDraw();

	//スタミナ処理。
	void Stamina();

	/// <summary>
	/// 空腹時のダメージ。
	/// </summary>
	void HungryDamage();

	//アイテムを投げる処理。
	void ThrowItem();

	void Shoulder();
	/// <summary>
	/// スペースをダブルクリックしたかどうか。
	/// </summary>
	/// <returns>doubleClickFlag</returns>
	bool SpaceDoubleClick();

	void Defence();

	//クリエイティブのときのパラメーター。
	void CreativeSt();

	/// <summary>
	/// 攻撃力算出
	/// </summary>
	void CalcAttackPow();

	bool m_isJump = false;					//飛んでいるか。
	bool m_doubleCilckFlag = false;			//ダブルクリックフラグ。
	bool m_runFlag = false;					//走っているか。
	bool m_flyingMode = true;				//クリエイティブの際、飛行モードかどうか。
	bool m_doubleClickFlagC = false;		//ダブルクリックフラグ(クリエイティブ)。
	bool m_flyingflag = false;				//飛べる状態か。
	bool m_attackFlag = false;				//エネミーに攻撃したか。
	bool m_deathFlag = false;				//死んだかどうか。
	bool m_eatingFlag = false;				//食べてるかどうか。
	bool m_isExpUpFlag = false;				//経験値アップした？

	bool  m_reverseTurnXZ = false;							//マウスによるXZ回転を反転するか。				
	float m_turnMult = 20.0f;								//プレイヤーの回転速度。

	float m_degreeY = 0.0f;									//Y軸の回転。
	float m_degreeXZ = 0.0f;								//XZ軸の回転。
	float m_radianY = 0.0f;									//Y軸の回転(ラジアン)。
	float m_radianXZ = 0.0f;								//XZ軸の回転(ラジアン)。
	float m_fallSpeed = 0.5f;								//プレイヤーの落下速度。
	float m_jmpInitialVelocity = 3.f;						//ジャンプの初速度。
	float m_runSpeedDouble = 1.f;							//走るときの移動速度の倍率。(走るときは2.fになる予定)。
	float m_doubleClickTimer = 0.0f;						//ダブルクリックの判定時間。
	float m_doubleClickTimerC = 0.0f;						//ダブルクリックの判定時間(クリエイティブ)。
	float m_deathAddRot = 0.f;								//死亡時の回転総数。
	float m_eatingTimer = 0.0f;
	const float m_gravity = 0.65f;							//重力。
	const float m_creativeSpeedMag = 3.f;					//クリエイティブの飛行中の移動速度の倍率。	
	const int installableBlockNum = 4;						//ブロック設置可能距離(ブロック距離)。

	int upDownY = 0;
	int FallDamage();		//落下ダメージ。

	float m_hp = 20.f;				//体力。
	float m_stamina = 20.000f;		//スタミナ。
	int m_attackPower = 5;			//攻撃力。
	int m_defensePower = 15;		//防御力。
	float m_exp = 0.f;				//経験値。
	int m_selItemNum = 0;			//プレイヤーが選択したアイテム番号（インベントリ番号）。

	CVector3 m_position = CVector3::One() * 15.0f* Block::WIDTH;				//プレイヤーの座標。
	CVector3 m_respawnPos = m_position;											//リスポーン座標。
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//右方向。
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//正面。	

	CQuaternion m_rotation = CQuaternion::Identity();			//クォータニオン。
	CQuaternion m_headBoneRot = CQuaternion::Identity();		//頭の骨の回転。
	CQuaternion m_shoulderBoneRot = CQuaternion::Identity();		//肩の骨の回転。

	Inventory m_inventory; //アイテムを保管するインベントリ。
	std::unique_ptr<GUI::RootNode> m_openedGUI; //現在開いているGUI

	enPlayerState m_playerState = enPlayerState_num;			//プレイヤーの状態。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//モデル。
	CRayTracingModelRender m_raytraceModel;						//レイトレモデル。
	MCCharaCon m_characon;										//キャラコン。
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//攻撃被弾判定用コリジョン。

	Bone* m_headBone;												//頭の骨。
	Bone* m_rightHandBone;											//右手の骨。
	Bone* m_shoulderBone;											//肩の骨。
	
	GameCamera* m_gameCamera = nullptr;							//ゲームカメラ。
	GameMode* m_gameMode = nullptr;								//ゲームモード。
	PlayerParameter* m_playerParameter = nullptr;				//プレイヤーのパラメーター。
	PlayerDeath* m_playerDeath = nullptr;						//プレイヤーの死亡時の画像処理。
	Game* m_game = nullptr;										//Gameクラス。
	PlayerArmor* m_playerArmor = nullptr;						//プレイヤーのアーマー。

	GameObj::CSkinModelRender m_blockCrackModel;				//ブロックのひび割れモデル
	float m_timerBlockDestruction = 0.0f;						//マウス長押しでブロック破壊する時のタイマー、一定時間経過でブロック破壊を実行する。
	bool m_isBlockDestruction = false;							//ブロック破壊をしたかどうか、平野が使う。
	float m_knockBackTimer = 0.0f;								//ノックバックのタイマー
	CVector3 m_knockBackDirection = CVector3::Zero();			//ノックバックの方向。
	float m_knockBack = 1.f;	//ノックバック感度。
	float m_knoceBackY = 1.f;	//ノックバックY座標。

	const wchar_t* m_damageName;		//ダメージ。
	const wchar_t* m_attackName;		//攻撃。
	const wchar_t* m_putName;			//物を置く。
	const wchar_t* m_walkName;			//歩く。
	const wchar_t* m_strikeName;		//叩く。

	float m_walkingTimer = 0.0f;//足音を鳴らす感覚用タイマー
};

