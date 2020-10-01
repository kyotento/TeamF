#pragma once
#include "../physics/character/CCharacterController.h"
class Player : public IGameObject
{
public:
	Player();
	~Player();

	bool Start();
	void Update();

	/// <summary>
	/// プレイヤーの移動処理。
	/// </summary>
	void Move();

	/// <summary>
	/// プレイヤーの回転処理。
	/// </summary>
	void Rotation();

	/// <summary>
	/// 座標を入手する。
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	CVector3 GetPosition()
	{
		return m_position;
	}


private:

	CVector3 m_position = CVector3::Zero();				//プレイヤーの座標。
	CVector3 m_charaPos = CVector3::Zero();				//キャラコンの座標。
	CVector3 m_scale = CVector3::One();					//プレイヤーモデルのスケール。

	CQuaternion m_rot = CQuaternion::Identity();		//プレイヤーの回転。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//プレイヤーのモデル。
	CCharacterController m_characon;					//キャラコン。


private://仮。

	GameObj::CSkinModelRender* m_skn = nullptr;			//仮床。


 };

