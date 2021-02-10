#include "stdafx.h"
#include "PlayerInventoryFiler.h"
#include "NullableItemStack.h"
#include "ItemStack.h" 
#include "Item.h"
#include "Player.h"

namespace
{
	const char* filePath = "Save/PlayerInventory.cag";
	const int inventorySize = 40;
}

 PlayerInventoryFiler::PlayerInventoryFiler() : m_inventory(inventorySize)
{

}

 bool PlayerInventoryFiler::LoadPlayerInventory()
 {
	 FILE* fp = NULL;
	 //�߂�l��0�Ȃ琳��ɊJ���Ă���̂ŁA0����Ȃ����͏������I������B
	 if (fopen_s(&fp, filePath, "rb") != 0) {
		 return false;
	 }
	 for (int i = 0; i < inventorySize; i++)
	 {
		 int itemId, number;
		 //fscanf�Ńt�@�C����ǂݍ��ށA�߂�l��EOF�������ꍇ�������I������
		 fscanf(fp, "%d %d", &itemId, &number);
		 //ID��None�łȂ���΁B
		 if (itemId != enCube_None)
		 {
			 auto& it = Item::GetItem(itemId);
			 //�A�C�e���𐶐����A�C���x���g���ɐݒ肷��B
			 auto item = std::make_unique<ItemStack>(Item::GetItem(itemId), number);
			 //auto item = std::make_unique<ItemStack>(Item::GetItem(i), number);
			 m_inventory.SetItem(i, std::move(item));
		 }
	 }
	 fscanf(fp, "%f %f %f" , &m_position.x, &m_position.y, &m_position.z);
	 fclose(fp);
	 return true;
 }

void PlayerInventoryFiler::SavePlayerInventory(Player* player)
{
	auto& inventory = player->GetInventory();
	std::string fileName = filePath;

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	for (int i = 0; i < inventorySize; i++) {
		NullableItemStack& item = inventory.GetNullableItem(i);
		int itemId = item.GetID();
		//unsigned�̍ő�l=null��������B
		if (itemId == enCube_None)
		{
			//ID��None�ɁB
			file << enCube_None << " ";
		}
		else
		{
			file << item.GetID() << " ";
		}
		file << item.GetNumber() << std::endl;
		
	}

	auto pos = player->GetPos();
	file << pos.x << " ";
	file << pos.y << " ";
	file << pos.z << std::endl;
	file.close();
}
