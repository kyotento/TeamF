#pragma once
#include "Player.h"
#include "../physics/character/CCharacterController.h"
#include "Entity.h"
#include "GameMode.h"

class Enemy : public Entity
{
public:
	Enemy();
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
		enEnemy_num,					//エネミーの状態の数。
	};

	//スケールの指定。
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}

protected:

	bool flag = false;//かり。

	int m_attackPow = 0;

	const float m_moveSpeed = 100.0f;				//移動速度。
	const float m_characonRadius = 40.f;			//キャラコンの半径。
	const float m_characonHeight = 160.f;			//キャラコンの高さ。
	const float m_interpolateTimeSec = 0.3f;		//アニメーション切り替え時のアニメーション補間時間。
	const float m_animSpeed = 1.0f;					//アニメーションの速度。
	float m_fallSpeed = 0.8f;						//落下速度。
	float m_jumpSpeed = 1.f;						//ジャンプ速度。
	float m_jmpInitialVelocity = 13.f;				//ジャンプの初速度。
	float m_gravity = 0.65f;						//重力。

	CVector3 m_position = CVector3::Zero();			//エネミーの座標。
	CVector3 m_scale = CVector3::One();				//プレイヤーのスケール。
	CVector3 m_characonPos = CVector3::Zero();		//キャラコンの座標。
	CVector3 m_characonMove = CVector3::Zero();		//キャラコンの移動量(向き持)。
	CVector3 m_direction = CVector3::Zero();		//Enemyから見たPlayerの向き。
	CVector3 m_oldDirection = CVector3::Zero();		//正規化する前の値を格納。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	AnimationClip m_animationClips[enAnimationClip_num];		//アニメーション。

	enEnemyState m_enemyState = enEnemy_num;					//エネミーの状態。

	CCharacterControllerType2 m_characon;						//キャラコン。
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//攻撃被弾判定用コリジョン。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。
	CRayTracingModelRender m_raytraceModel;						//レイトレモデル。
	Player* m_player = nullptr;									//プレイヤー。
	GameMode* m_gameMode = nullptr;								//ゲームモード。

};

