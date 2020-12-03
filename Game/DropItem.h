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

	void Drop(World* world);		//�A�C�e���h���b�v�B

	/// <summary>
	/// ���������B
	/// </summary>
	void Fall();

	/// <summary>
	///	��]�����B
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
	int m_rotAmount = 0;			//��]�ʁB

	CVector3 m_position = CVector3::Zero();
	CVector3 m_colPos = CVector3::Zero();			//�����蔻��̍��W�B
	CVector3 m_colScale = CVector3::One();			//�����蔻��̃X�P�[���B

	CQuaternion m_rot = CQuaternion::Identity();	//��]�B

	EnCube m_state = enCube_None;
	Box* m_box = nullptr;

	CVector3 m_velocity; //���x

	World* m_world = nullptr; //���������݂��郏�[���h

	SimpleMoveObj m_collision;//�����蔻��B
};

