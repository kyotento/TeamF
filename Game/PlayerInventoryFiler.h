#pragma once
#include "Inventory.h"

class Player;
class PlayerInventoryFiler
{
public:
	PlayerInventoryFiler();
	~PlayerInventoryFiler() {}
	//プレイヤーインベントリをロードする。
	//ロードできなければ、falseを返す。
	bool LoadPlayerInventory();

	//プレイヤーインベントリをセーブする。
	void SavePlayerInventory(Player* player);

	//ロードしたインベントリを取得。
	Inventory& GetInventory()
	{
		return m_inventory;
	}
	//ポジションを取得。
	const CVector3& GetPosition() const
	{
		return m_position;
	}
private:
	Inventory m_inventory;
	CVector3 m_position;
};

