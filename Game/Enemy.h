#pragma once

class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	//bool Start();
	//void Update();

	//エネミーのアニメーション。
	enum enAnimationClips {
		enAnimationClip_idle,
		enAnimationClip_move,
		enAnimationClip_tracking,
		enAnimationClip_attack,
		enAnimationClip_num
	};

	//エネミーの状態。
	enum enEnemyState {
		enEnemy_idle,
		enEnemy_move,
		enEnemy_tracking,
		enEnemy_attack,
		enEnemy_num,
	};


protected:

	CVector3 m_position = CVector3::Zero();			//座標。
	CVector3 m_scale = CVector3::One();				//プレイヤーのスケール。

	CQuaternion m_rot = CQuaternion::Identity();	//回転情報。

	enAnimationClips m_animationClips[enAnimationClip_num];		//アニメーション。
	enEnemyState m_enemyState = enEnemy_num;					//エネミーの状態。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデル。

};

