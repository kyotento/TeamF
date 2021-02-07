#pragma once
#include "Inventory.h"

class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();

	//�v���C���[�C���x���g�������[�h����B
	bool LoadPlayerInventory();

	//�v���C���[�C���x���g�����Z�[�u����B
	void SavePlayerInventory(Inventory& inventory);

	Inventory& GetPlayerInventory()
	{
		return m_inventory;
	}
private:
	Inventory m_inventory;
};

