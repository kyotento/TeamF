#pragma once
#include "Player.h"
#include "../physics/character/CCharacterController.h"

class Enemy : public IGameObject
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

	//状態管理。
	virtual void StateManagement();

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
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
protected:

	const float m_moveSpeed = 100.0f;					//移動速度。
	const float m_characonRadius = 40.f;			//キャラコンの半径。
	const float m_characonHeight = 160.f;			//キャラコンの高さ。
	const float m_interpolateTimeSec = 0.3f;		//アニメーション切り替え時のアニメーション補間時間。
	const float m_animSpeed = 1.0f;					//アニメーションの速度。

	CVector3 m_position = CVector3::Zero();			//エネミーの座標。
	CVector3 m_scale = CVector3::One();				//プレイヤーのスケール。
	CVector3 m_characonPos = CVector3::Zero();		//キャラコンの座標。
	CVector3 m_characonMove = CVector3::Zero();		//キャラコンの移動量(向き持)。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	AnimationClip m_animationClips[enAnimationClip_num];		//アニメーション。

	enEnemyState m_enemyState = enEnemy_num;					//エネミーの状態。

	CCharacterControllerType2 m_characon;							//キャラコン。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。
	Player* m_player = nullptr;

};

