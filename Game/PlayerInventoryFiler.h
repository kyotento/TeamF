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

	NullableItemStack& GetItem(const int slot)
	{
		return m_inventory.GetNullableItem(slot);
	}
private:
	Inventory m_inventory;
};

