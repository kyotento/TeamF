#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"
#include "Entity.h"
#include "Inventory.h"

class World;
class GameCamera;
class Item;
class GameMode;
class ItemDisplay;
class PlayerParameter;
class PlayerDeath;
class Game;
namespace GUI{
	class RootNode;
}

class Player : public Entity
{
public:
	Player(World* world);
	~Player();
	bool Start() override;
	void Update() override;

	void Test();//todo　リリースのときに消します。

	CFont font;
	void HUDRender( int HUDNum ) override{
		std::wstringstream str;
		CVector3 pos = GetPos() / Block::WIDTH;
		str << pos.x << " , " << pos.y << " , " << pos.z << "\n";
		font.Draw( str.str().c_str(), { 0.9f , 0.1f }, CVector4::White(), 0.5f, {0.5f, 0.5f} );
	}

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
		enPlayerState_death,			//死んだとき。
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

	//! @brief 回転を取得。
	CQuaternion GetRot() const {
		return m_rotation;
	}

	//! @brief 座標を設定。
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
	}

	/// <summary>
	/// HPを取得する。
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHP()
	{
		return m_hp;
	}

	/// <summary>
	/// スタミナを取得する。
	/// </summary>
	/// <returns>スタミナ</returns>
	const int& GetStamina()
	{
		return m_stamina;
	}

	/// <summary>
	/// 防御力を取得。
	/// </summary>
	/// <returns>防御力</returns>
	const int& GetDefPow()
	{
		return m_defensePower;
	}

	/// <summary>
	/// 経験値を取得する。
	/// </summary>
	/// <returns>経験値</returns>
	const float& GetExp()
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
	const int& GetSelectItemNum()
	{
		return m_selItemNum;
	}

	/// <summary>
	/// プレイヤーのY軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのY軸の回転</returns>
	float GetRadianY()
	{
		return m_radianY;
	}

	/// <summary>
	/// プレイヤーのXZ軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのXZ軸の回転</returns>
	float GetRadianXZ()
	{
		return m_radianXZ;
	}

	World* GetWorld(){
		return m_world;
	}

	//! @brief World をセットする。
	//! @param recursive trueなら World::SetPlayer(this, false) も呼び出す。
	void SetWorld( World* world , bool recursive = false);

	//インベントリの長さ
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;

	Inventory& GetInventory(){
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
	/// 被ダメージ
	/// </summary>
	/// <param name="AttackePow">攻撃力</param>
	void TakenDamage(int AttackePow);

	/// <summary>
	/// ゲームのインスタンスを設定する。
	/// </summary>
	/// <param name="game"></param>
	void SetGameIns(Game* game)
	{
		m_game = game;
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
	/// <param name="ray">当たったオブジェクトの判定</param>
	/// <param name="frontRotAdd">プレイヤーの回転</param>
	void InstallAndDestruct(btCollisionWorld::ClosestRayResultCallback ray , CVector3 frontRotAdd);

	/// <summary>
	/// プレイヤーの前方にレイを飛ばす。
	/// </summary>
	void FlyTheRay();

	/// <summary>
	/// 死亡処理。
	/// </summary>
	void Death();

	/// <summary>
	/// リスポーン。
	/// </summary>
	void Respawn();

	/// <summary>
	/// 右手表示の更新してます。
	/// </summary>
	void ItemDisplayUpdate();

	/// <summary>
	/// スペースをダブルクリックしたかどうか。
	/// </summary>
	/// <returns>doubleClickFlag</returns>
	bool SpaceDoubleClick();

	bool m_isJump = false;					//飛んでいるか。
	bool m_doubleCilckFlag = false;			//ダブルクリックフラグ。
	bool m_runFlag = false;					//走っているか。
	bool m_flyingMode = true;				//クリエイティブの際、飛行モードかどうか。
	bool m_doubleClickFlagC = false;		//ダブルクリックフラグ(クリエイティブ)。
	bool m_flyingflag = false;				//飛べる状態か。
	bool m_attackFlag = false;				//エネミーに攻撃したか。

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
	const float m_gravity = 0.65f;							//重力。
	const float m_creativeSpeedMag = 3.f;					//クリエイティブの飛行中の移動速度の倍率。	
	const int installableBlockNum = 4;						//ブロック設置可能距離(ブロック距離)。

	int m_hp = 20;					//体力。
	int m_stamina = 20;				//スタミナ。
	int m_attackPower = 5;			//攻撃力。
	int m_defensePower = 15;		//防御力。
	float m_exp = 5.50f;			//経験値。
	int m_selItemNum = 0;			//プレイヤーが選択したアイテム番号（インベントリ番号）。

	CVector3 m_position = CVector3::One() * 15.0f* Block::WIDTH;				//プレイヤーの座標。
	CVector3 m_respawnPos = m_position;											//リスポーン座標。
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//右方向。
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//正面。	

	CQuaternion m_rotation = CQuaternion::Identity();			//クォータニオン。
	CQuaternion m_headBoneRot = CQuaternion::Identity();		//頭の骨の回転。

	Inventory m_inventory; //アイテムを保管するインベントリ。
	std::unique_ptr<GUI::RootNode> m_openedGUI; //現在開いているGUI

	enPlayerState m_playerState = enPlayerState_num;			//プレイヤーの状態。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//モデル。
	CRayTracingModelRender m_raytraceModel;						//レイトレモデル。
	CCharacterControllerType2 m_characon;						//キャラコン。
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//攻撃被弾判定用コリジョン。

	Bone* m_headBone;												//頭の骨。

	GameCamera* m_gameCamera = nullptr;							//ゲームカメラ。
	GameMode* m_gameMode = nullptr;								//ゲームモード。
	ItemDisplay* m_rightHandDisplay = nullptr;					//右手表示。
	PlayerParameter* m_playerParameter = nullptr;				//プレイヤーのパラメーター。
	PlayerDeath* m_playerDeath = nullptr;						//プレイヤーの死亡時の画像処理。
	Game* m_game = nullptr;										//Gameクラス。
};

