#pragma once
#include "BlockType.h"


class Player;
class Box;
class BlockFactory;
class DropItem : public IGameObject
{
public:
	DropItem() {}
	~DropItem() 
	{
		DeleteGO(m_model);
	}
	bool Start() override;
	void Update() override;

	void Drop();		//アイテムドロップ。

	/// <summary>
	/// 落下処理。
	/// </summary>
	void Fall();

	/// <summary>
	///	回転処理。
	/// </summary>
	void Rotation();

	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	void SetNumber(int number)
	{
		m_number = number;
	}
	void SetEnCube(EnCube enCube)
	{
		m_state = enCube;
	}
private:
	void Distance();
private:
	GameObj::CSkinModelRender* m_model;

	int m_number = 0;
	int m_rotAmount = 0;			//回転量。

	CVector3 m_position = CVector3::Zero();
	CVector3 m_colPos = CVector3::Zero();			//当たり判定の座標。
	CVector3 m_colScale = CVector3::One();			//当たり判定のスケール。

	CQuaternion m_rot = CQuaternion::Identity();	//回転。

	EnCube m_state = enCube_None;
	Player* m_player = nullptr;
	Box* m_box = nullptr;

	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;		//当たり判定。
};

