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

	//ロードしたインベントリを取得。
	Inventory& GetInventory()
	{
		return m_inventory;
	}
private:
	Inventory m_inventory;
};

