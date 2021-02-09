#pragma once
#include "Block.h"
#include "Inventory.h"

class Player;

class Chest :public Block {
public:
	Chest();
	~Chest();
	bool OnClick(Player* player) override;

	//プレイヤーに破壊された時。
	void DestroyedPlayer() override final
	{
		//ドロップアイテムを生成する。
		GenerateDropItem();
	}

	//ドロップアイテムを生成する。
	void GenerateDropItem();

private:
	//3×9の27個のスロット。
	Inventory m_inventory;
};