#include "stdafx.h"
#include "CraftingController2.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"
#include "DropItem.h"
#include "Block.h"
#include "Player.h"

namespace GUI::Controller {
	CraftingController2::CraftingController2(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

	}

	CraftingController2::~CraftingController2() {
		const unsigned resultSlot = 4;
		//GUIを閉じたときにクラフトテーブルのアイテムを返却する。
		int number = 0;
		for (auto& item : m_inventory) 
		{	//クラフトスロットのところだけアイテムを返却する。
			if (number <= resultSlot - 1) {
				//アイテムをプレイヤーインベントリに返却できなかったら。
				if (m_inventory.GetNullableItem(number).GetID() != enCube_None && !m_playerInventory.AddItem(item))
				{
					//ドロップアイテムを生成する。
					auto player = FindGO<Player>();
					if (player != nullptr)
					{
						player->CreateFrontDropItem(item);
					}

				}
			}
			number++;
		}
	}

	void CraftingController2::OnClickSlot(Event::ClickEvent & event, unsigned slotNo) {
		const unsigned resultSlot = 4;

		if (slotNo == resultSlot) {
			//完成品スロットをクリックしたときの動作。
			auto& result = m_inventory.GetItem(resultSlot);

			if (result == nullptr) {
				return;
			}
			if (m_grabed == nullptr) {
				//カーソルが空なら個数0の完成品と同じアイテムを持たせる。
				m_grabed = std::make_unique<ItemStack>(result->GetItem(), 0);
			}
			if (m_grabed->GetID() != result->GetID()) {
				return;
			}

			//アイテム移動の処理
			const int grabNum = m_grabed->GetNumber();
			const int resultNum = result->GetNumber();

			//カーソルのアイテムがスタック上限を超えそうなら、取得はできない。
			if( grabNum + resultNum > m_grabed->GetStackLimit() ){
				return;
			}

			m_grabed->SetNumber( grabNum + resultNum );
			m_inventory.SetItem( resultSlot, nullptr );

			//完成品を取ったので、クラフトテーブルに置いてあるアイテムを消費する。
			for (int i = 0; i <= 3; i++) {
				auto& item = m_inventory.GetItem(i);

				if (item) {
					item->SetNumber(item->GetNumber() - 1);

					if (item->GetNumber() == 0) {
						m_inventory.SetItem(i, nullptr);
					}
				}
			}
		}

		if (slotNo < resultSlot) {
			//通常のクリック操作。
			InventoryController::OnClickSlot(event, slotNo);
		}

		//クラフト結果を検索。成果物スロットを更新する。
		auto& rm = RecipeManager::Instance();

		int itemArray[4];
		for (int i = 0; i < 4; i++) {

			auto& item = m_inventory.GetItem(i);

			if (item) {
				itemArray[i] = item->GetID();
			}
			else {
				//nullをアイテム無しを表すenumに置き換える。
				itemArray[i] = enCube_None;
			}

		}

		auto result = rm.GetResult(itemArray);

		m_inventory.SetItem(4, std::move(result));
	}
}