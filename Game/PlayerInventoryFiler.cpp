#include "stdafx.h"
#include "PlayerInventoryFiler.h"
#include "NullableItemStack.h"
#include "ItemStack.h"

#include "Player.h"
#include "Game.h"
#include "GameCamera.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "Item.h"
#include "GameMode.h"
#include "World.h"
#include "PlayerInventory.h"
#include "BlockFactory.h"
#include "DamegeScreenEffect.h"
#include "Enemy.h"
#include "PlayerParameter.h"
#include "PlayerDeath.h"
#include "Menu.h"
#include "DropItem.h"
#include"Animals.h"
#include "PlayerArmor.h"

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
	 //戻り値が0なら正常に開けているので、0じゃない時は処理を終了する。
	 if (fopen_s(&fp, filePath, "rb") != 0) {
		 return false;
	 }
	 for (int i = 0; i < inventorySize; i++)
	 {
		 int itemId, number;
		 //fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
		 fscanf(fp, "%d %d", &itemId, &number);
		 //IDがNoneでなければ。
		 if (itemId != enCube_None)
		 {
			 auto& it = Item::GetItem(itemId);
			 //アイテムを生成し、インベントリに設定する。
			 //auto item = std::make_unique<ItemStack>(Item::GetItem(itemId), number);
			 auto item = std::make_unique<ItemStack>(Item::GetItem(1), number);
			 m_inventory.SetItem(i, std::move(item));
			 //m_inventory.AddItem(item);
		 }
	 }
	 fclose(fp);
	 return true;
 }

void PlayerInventoryFiler::SavePlayerInventory(Inventory& inventory)
{
	std::string fileName = filePath;

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	for (int i = 0; i < inventorySize; i++) {
		NullableItemStack& item = inventory.GetNullableItem(i);
		int itemId = item.GetID();
		//unsignedの最大値=nullだったら。
		if (itemId == enCube_None)
		{
			//IDをNoneに。
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
