#include "stdafx.h"
#include "Chest.h"
#include "Player.h"
#include "ChestInventory.h"
#include "DropItem.h"
#include "ItemStack.h"

namespace {
	const int randomDrop = Block::WIDTH / 0.5;	//らんちゅうのはんい。
	std::mt19937 random((std::random_device())());	//らんちゅう。
}

Chest::Chest() : m_inventory(27)
{

}

Chest::~Chest()
{
	
}

bool Chest::OnClick(Player * player) {
	player->OpenGUI(std::make_unique<GUI::ChestInventory>(player,m_inventory));
	return true;
}

void Chest::GenerateDropItem()
{
	for (int i = 0; i < 27; i++) {
		auto item = m_inventory.TakeAllItem(i);
		if (item) {
			CVector3 pos = GetModelPos();
			pos.y += Block::WIDTH;
			DropItem* drop = DropItem::CreateDropItem(m_sWorld, std::move(item));
			CVector3 addPos = CVector3::Zero();
			if (random() % 2 > 0) {
				addPos.x += rand() % randomDrop;
			}
			else {
				addPos.x -= rand() % randomDrop;
			}

			if (random() % 2 > 0) {
				addPos.z += rand() % randomDrop;
			}
			else {
				addPos.z += rand() % randomDrop;
			}
			drop->SetPos(pos + addPos);
		}
	}
}

void Chest::ReadExData( std::ifstream & ifs ){
	m_inventory.ReadData( ifs );
}

void Chest::WriteExData( std::ofstream & ofs ){
	m_inventory.WriteData( ofs );
}
