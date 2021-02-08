#pragma once
#include "Inventory.h"


class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();
	~PlayerInventoryFiler() {}
	//�v���C���[�C���x���g�������[�h����B
	//���[�h�ł��Ȃ���΁Afalse��Ԃ��B
	bool LoadPlayerInventory();

	//�v���C���[�C���x���g�����Z�[�u����B
	void SavePlayerInventory(Inventory& inventory);

	//���[�h�����C���x���g�����擾�B
	Inventory& GetInventory()
	{
		return m_inventory;
	}
private:
	Inventory m_inventory;
};

