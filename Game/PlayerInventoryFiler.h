#pragma once
#include "Inventory.h"

class Player;
class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();
	~PlayerInventoryFiler() {}
	//�v���C���[�C���x���g�������[�h����B
	//���[�h�ł��Ȃ���΁Afalse��Ԃ��B
	bool LoadPlayerInventory();

	//�v���C���[�C���x���g�����Z�[�u����B
	void SavePlayerInventory(Player* player);

	//���[�h�����C���x���g�����擾�B
	Inventory& GetInventory()
	{
		return m_inventory;
	}
	//�|�W�V�������擾�B
	const CVector3& GetPosition() const
	{
		return m_position;
	}
private:
	Inventory m_inventory;
	CVector3 m_position;
};

