#pragma once
#include "Inventory.h"

class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();

	//プレイヤーインベントリをロードする。
	bool LoadPlayerInventory();

	//プレイヤーインベントリをセーブする。
	void SavePlayerInventory(Inventory& inventory);

	Inventory& GetPlayerInventory()
	{
		return m_inventory;
	}
private:
	Inventory m_inventory;
};

