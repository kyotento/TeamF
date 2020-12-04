#pragma once
#include "BlockType.h"
#include "Entity.h"
#include "SimpleMoveObj.h"

class Player;
class Box;
class BlockFactory;
class World;

class DropItem : public Entity
{
public:
	DropItem() {}
	~DropItem();
	bool Start() override;
	void Update() override;

	void Drop(World* world);		//アイテムドロップ。

	/// <summary>
	/// 落下処理。
	/// </summary>
	void Fall();

	/// <summary>
	///	回転処理。
	/// </summary>
	void Rotation();

	void SetPos( const CVector3& position ) override;

	CVector3 GetPos() const override{
		return m_position;
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
	CRayTracingModelRender m_raytraceModel;

	int m_number = 0;
	int m_rotAmount = 0;			//回転量。

	CVector3 m_position = CVector3::Zero();
	CVector3 m_colPos = CVector3::Zero();			//当たり判定の座標。
	CVector3 m_colScale = CVector3::One();			//当たり判定のスケール。

	CQuaternion m_rot = CQuaternion::Identity();	//回転。

	EnCube m_state = enCube_None;
	Box* m_box = nullptr;

	CVector3 m_velocity; //速度

	World* m_world = nullptr; //自分が存在するワールド

	SimpleMoveObj m_collision;//当たり判定。
};

