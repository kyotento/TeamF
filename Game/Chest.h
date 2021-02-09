#pragma once
#include "Block.h"
#include "Inventory.h"

class Player;

class Chest :public Block {
public:
	Chest();
	~Chest();
	bool OnClick(Player* player) override;

	//�v���C���[�ɔj�󂳂ꂽ���B
	void DestroyedPlayer() override final
	{
		//�h���b�v�A�C�e���𐶐�����B
		GenerateDropItem();
	}

	//�h���b�v�A�C�e���𐶐�����B
	void GenerateDropItem();

private:
	//3�~9��27�̃X���b�g�B
	Inventory m_inventory;
};