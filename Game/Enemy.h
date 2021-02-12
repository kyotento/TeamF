#pragma once
#include "Player.h"
#include "../physics/character/CCharacterController.h"
#include "GameMode.h"
#include "Mob.h"
#include "Game.h"
class World;
class Enemy : public Mob
{
public:
	Enemy(EnEntity enEntity = enEntity_None);
	virtual ~Enemy();

	/// <summary>
	/// 何かを追跡する処理。
	/// </summary>
	/// <param name="pos">追跡させるEnemyの座標</param>
	/// プレイヤーを追従するEnemyの子クラスで呼んであげてください。
	virtual void Tracking();

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
	/// 太陽からのダメージ
	/// </summary>
	virtual void AttackSun();

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

	//! @brief 座標を取得。
	CVector3 GetPos() const override{
		return m_position;
	}

	//! @brief 座標を設定。
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
		m_characonPos = pos;
	}

	/// <summary>
	/// エンティティの有効切替時の処理
	/// </summary>
	/// <param name="enable"></param>
	void SetEnableEntityInner(bool enable)override {
		m_damageCollision->SetEnable(enable);
		m_skinModelRender->SetEnable(enable);
	}

	//エネミーのアニメーション。
	enum enAnimationClips {
		enAnimationClip_idle,			//待機。
		enAnimationClip_move,			//移動。
		enAnimationClip_tracking,		//追跡。
		enAnimationClip_attack,			//攻撃。
		enAniamtionClip_fan,			//煽り。
		enAnimationClip_num				//アニメーションの数。
	};

	//エネミーの状態。
	enum enEnemyState {
		enEnemy_idle,					//待機。
		enEnemy_move,					//移動。
		enEnemy_tracking,				//追跡。
		enEnemy_attack,					//攻撃。
		enEnemy_fan,					//煽り。		
		enEnemy_death,					//死んだとき。
		enEnemy_num,					//エネミーの状態の数。
	};

	//スケールの指定。
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}

protected:

	const wchar_t* m_damageVoice;		//ダメージ音。
	const wchar_t* m_deathVoice;		//死亡音。

	bool m_jumpFlag = false;			//ジャンプするどうか。
	bool m_isTakenDamage = false;		//ダメージを受けた時。
	//todo 現時点では、ダメージフラグを使えばいい。
	bool m_isInvincibleTime = false;	//無敵時間かどうか。

	int m_hp = 0;				//体力。
	int m_attackPow = 0;		//攻撃力。
	float m_exp = 0.f;			//取得経験値量。
	float m_knockBack = 1.f;	//ノックバック感度。
	float m_knoceBackY = 1.f;	//ノックバックY座標。

	const float m_moveSpeed = 100.0f;				//移動速度。
	const float m_characonRadius = 40.f;			//キャラコンの半径。
	const float m_characonHeight = 160.f;			//キャラコンの高さ。
	const float m_interpolateTimeSec = 0.3f;		//アニメーション切り替え時のアニメーション補間時間。
	const float m_animSpeed = 1.0f;					//アニメーションの速度。
	float m_fallSpeed = 0.8f;						//落下速度。
	float m_jumpSpeed = 1.f;						//ジャンプ速度。
	float m_jmpInitialVelocity = 13.f;				//ジャンプの初速度。
	float m_gravity = 0.65f;						//重力。
	float m_deathAddRot = 0.f;						//死亡じの回転総数。
	float m_knockBackTimer = 0.f;					//ノックバックタイマー。

	CVector3 m_position = CVector3::Zero();			//エネミーの座標。
	CVector3 m_scale = CVector3::One();				//プレイヤーのスケール。
	CVector3 m_characonPos = CVector3::Zero();		//キャラコンの座標。
	CVector3 m_characonMove = CVector3::Zero();		//キャラコンの移動量(向き持)。
	CVector3 m_direction = CVector3::Zero();		//Enemyから見たPlayerの向き。
	CVector3 m_oldDirection = CVector3::Zero();		//正規化する前の値を格納。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	AnimationClip m_animationClips[enAnimationClip_num];		//アニメーション。

	enEnemyState m_enemyState = enEnemy_num;					//エネミーの状態。

	MCCharaCon m_characon;												//キャラコン。
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//攻撃被弾判定用コリジョン。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。
	CRayTracingModelRender m_raytraceModel;						//レイトレモデル。
	Player* m_player = nullptr;									//プレイヤー。
	GameMode* m_gameMode = nullptr;								//ゲームモード。
	Game* m_game = nullptr;										//Game。

	float m_sunDamageTimer = 0.0f;
	Sun* m_sun = nullptr;
};

