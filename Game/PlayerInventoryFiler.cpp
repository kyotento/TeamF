#include "stdafx.h"
#include "PlayerInventoryFiler.h"
#include "NullableItemStack.h"
#include "ItemStack.h"

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
	return true;
}

void PlayerInventoryFiler::SavePlayerInventory(Inventory& inventory)
{
	std::string fileName = filePath;

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	if (!file)
	{
		return;
	}


	for (int i = 0; i < inventorySize; i++) {
		NullableItemStack& item = inventory.GetNullableItem(i);
		int itemId = item.GetID();
		//unsigned‚ÌÅ‘å’l=null‚¾‚Á‚½‚çB
		if (itemId == 4294967295)
		{
			//ID‚ðNone‚ÉB
			file << enCube_None << " ";
		}
		else
		{
			file << item.GetID() << " ";
		}
		file << item.GetNumber() << std::endl;
		
	}
	file.close();
}
