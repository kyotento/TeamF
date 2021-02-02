#pragma once
#include"Player.h"
#include"GameMode.h"
#include "../physics/character/CCharacterController.h"
#include "Mob.h"

class Animals : public Mob
{
public:
	Animals(EnEntity enEntity = enEntity_None);
	virtual ~Animals();

	/// <summary>
	/// 状態管理。
	/// </summary>
	virtual void StateManagement();

	/// <summary>
	/// 落下処理。
	/// </summary>
	virtual void Fall();

	/// <summary>
	/// ジャンプ処理。
	/// </summary>
	virtual void Jump();

	/// <summary>
	/// 被ダメージ処理。
	/// </summary>
	/// <param name="attackDamage">攻撃力</param>
	virtual void TakenDamage(int attackDamage);

	//被ダメ時のダメージ音。
	virtual void DamageVoice();

	/// <summary>
	/// ノックバック処理。
	/// </summary>
	virtual void KnockBack();

	/// <summary>
	/// 死亡時の処理。
	/// </summary>
	virtual void Death();

public:
	/// <summary>
	/// 座標を取得。
	/// </summary>
	CVector3 GetPos() const {
		return m_position;
	}
	/// <summary>
	/// 座標を設定。
	/// </summary>
	void SetPos(const CVector3& pos){
		m_position = pos;
		m_characon.SetPosition(pos);
		m_characonPos = pos;
	}

	//スケールの指定。
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}

	//動物の状態。
	enum enAnimalsState {
		enAnimals_idle,					//待機。
		enAnimals_move,					//移動。
		enAnimals_escape,					//逃亡。
		enAnimals_death,					//死亡。
	};

protected:

	const wchar_t* m_damageVoice;		//ダメージ音。
	const wchar_t* m_deathVoice;		//死亡音。

	bool m_jumpFlag = false;			//ジャンプするどうか。
	bool m_isTakenDamage = false;		//ダメージを受けた時。
	bool m_isInvincibleTime = false;	//無敵時間かどうか。

	int m_hp = 0;						//体力。
	float m_knockBack = 1.f;			//ノックバック感度。
	float m_knoceBackY = 1.f;			//ノックバックY座標。

	const float m_moveSpeed = 100.0f;				//移動速度。
	const float m_characonRadius = 40.f;			//キャラコンの半径。
	const float m_characonHeight = 160.f;			//キャラコンの高さ。

	float m_fallSpeed = 0.8f;						//落下速度。
	float m_jumpSpeed = 1.f;						//ジャンプ速度。
	float m_jmpInitialVelocity = 13.f;				//ジャンプの初速度。
	float m_gravity = 0.65f;						//重力。
	float m_deathAddRot = 0.f;						//死亡時の回転総数。
	float m_knockBackTimer = 0.f;					//ノックバックタイマー。

	CVector3 m_position = CVector3::Zero();			//動物の座標。
	CVector3 m_scale = CVector3::One();				//動物のスケール。
	CVector3 m_characonPos = CVector3::Zero();		//キャラコンの座標。
	CVector3 m_characonMove = CVector3::Zero();		//キャラコンの移動量(向き持)。
	CVector3 m_direction = CVector3::Zero();		//Animalから見たPlayerの向き。
	CVector3 m_oldDirection = CVector3::Zero();		//正規化する前の値を格納。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	enAnimalsState m_animalState = enAnimals_idle;						//状態。

	MCCharaCon m_characon;								//キャラコン。
	CRayTracingModelRender m_raytraceModel;								//レイトレモデル。
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//攻撃被弾判定用コリジョン。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。
	Player* m_player = nullptr;									//プレイヤー。
	GameMode* m_gameMode = nullptr;								//ゲームモード。

	int a = 0;
};

