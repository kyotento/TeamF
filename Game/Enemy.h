#pragma once
#include "Player.h"

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
	virtual void Tracking(/*const CVector3 pos*/);

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

protected:

	const float m_moveSpeed = 1.0f;					//移動速度。

	CVector3 m_position = CVector3::Zero();			//座標。
	CVector3 m_scale = CVector3::One();				//プレイヤーのスケール。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	AnimationClip m_animationClips[enAnimationClip_num];		//アニメーション。

	enEnemyState m_enemyState = enEnemy_num;					//エネミーの状態。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。
	Player* m_player = nullptr;

};

