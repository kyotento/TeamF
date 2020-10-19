#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"


class GameCamera;
class Item;
class GameMode;
struct Inventory {
	Item* s_item = nullptr;
	int s_number = 0;
};

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;

	/// <summary>
	/// アニメーション。
	/// </summary>
	enum enAnimationClip{
		enAnimationClip_Idle,			//待機状態。
		enAnimationClip_move,			//移動。
		enAnimationClip_shift,			//しゃがみ。
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
		enPlayerState_shift,			//しゃがみ。
		enPlayerState_excavate,			//物を掘る。
		enPlayerState_num,				//状態の数。
	};

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
	
	/// <summary>
	/// プレイヤーの座標を取得。
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	const CVector3& GetPosition()
	{
		return m_position;
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
	//インベントリの長さ
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;
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
	/// プレイヤーの状態管理。
	/// </summary>
	void StateManagement();

	bool m_isJump = false;					//飛んでいるか。
	bool m_doubleCilckFlag = false;			//ダブルクリックフラグ。
	bool m_runFlag = false;					//走っているか。

	float m_degreeY = 0.0f;									//Y軸の回転。
	float m_degreeXZ = 0.0f;								//XZ軸の回転。
	float m_radianY = 0.0f;									//Y軸の回転(ラジアン)。
	float m_radianXZ = 0.0f;								//XZ軸の回転(ラジアン)。
	float m_fallSpeed = 0.5f;								//プレイヤーの落下速度。
	float m_jmpInitialVelocity = 3.f;						//ジャンプの初速度。
	float m_runSpeedDouble = 1.f;							//走るときの移動速度の倍率。(走るときは2.fになる予定)。
	float m_doubleClickTimer = 0.0f;						//ダブルクリックの判定時間。
	const float m_characonRadius = 50.f;					//キャラコンの半径。
	const float m_characonHeight = 160.f;					//キャラコンの高さ。
	const float m_gravity = 0.65f;							//重力。

	CVector3 m_position = CVector3::One() * 15.0f* Block::WIDTH;				//プレイヤーの座標。
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//右方向。
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//正面。	

	CQuaternion m_rotation = CQuaternion::Identity();			//クォータニオン。

	std::vector<Inventory*> m_inventoryList;					//インベントリ。

	enPlayerState m_playerState = enPlayerState_num;			//プレイヤーの状態。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//モデル。
	CCharacterControllerType2 m_characon;							//キャラコン。
	CFont m_font;												//フォント。
	GameCamera* m_gameCamera = nullptr;							//ゲームカメラ。
	GameMode* m_gameMode = nullptr;								//ゲームモード。
};

