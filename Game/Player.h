#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"


class GameCamera;
class Item;
struct Inventory {
	Item* s_item = nullptr;
	int s_number = 0;
};

class Player : public IGameObject
{
public:
	Player() {}
	~Player() {}
	bool Start() override;
	void Update() override;
	void PostRender() override;
	//�E�������擾
	const CVector3& GetRight()
	{
		return m_right;
	}
	//���ʕ������擾
	const CVector3& GetFront()
	{
		return m_front;
	}
	//���W���擾
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//Y���̉�]���擾
	float GetRadianY()
	{
		return m_radianY;
	}
	//XZ���̉�]���擾
	float GetRadianXZ()
	{
		return m_radianXZ;
	}
	//�C���x���g���̒���
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;
private:
	//�ړ�
	void Move();
	//��]
	void Turn();
public:
	CVector3 m_position = CVector3::One() * 15.0f;				//���W
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//�E����
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//����
	GameCamera* m_gameCamera;									//�Q�[���J����
	float m_degreeY = 0.0f;										//Y���̉�]
	float m_degreeXZ = 0.0f;									//XZ���̉�]
	float m_radianY = 0.0f;										//Y���̉�](���W�A��)
	float m_radianXZ = 0.0f;									//XZ���̉�](���W�A��)
	CQuaternion m_rotation = CQuaternion::Identity();			//�N�H�[�^�j�I��
	GameObj::CSkinModelRender m_model;							//���f��
	CCharacterController m_characon;							//�L�����R��
	float m_characonRadius = 50.f;			//�L�����R���̔��a�B
	float m_characonHeight = 100.f;			//�L�����R���̍����B
	CFont m_font;												//�t�H���g
	std::vector<Inventory*> m_inventoryList;					//�C���x���g��
};

