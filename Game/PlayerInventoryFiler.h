#pragma once
#include "Inventory.h"


class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();
	~PlayerInventoryFiler() {}
	//プレイヤーインベントリをロードする。
	//ロードできなければ、falseを返す。
	bool LoadPlayerInventory();

	//プレイヤーインベントリをセーブする。
	void SavePlayerInventory(Inventory& inventory);

	NullableItemStack& GetItem(const int slot)
	{
		return m_inventory.GetNullableItem(slot);
	}
private:
	Inventory m_inventory;
};

