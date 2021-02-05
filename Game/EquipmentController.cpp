#include "stdafx.h"
#include "EquipmentController.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace GUI::Controller {
	EquipmentController::EquipmentController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

	}

	EquipmentController::~EquipmentController() {
		//GUIを閉じたときにクラフトテーブルのアイテムを返却する。
		for (auto& item : m_inventory) {
			m_playerInventory.AddItem(item);
		}
	}

	void EquipmentController::OnClickSlot(Event::ClickEvent & event, unsigned slotNo) {
		const unsigned craftSlot = 4;

		//クラフトスロットより上なら操作する。
		if (slotNo > craftSlot) {
			//装備スロットをクリックしたときの動作。
			auto& result = m_inventory.GetItem(slotNo);
			//装備品が既に存在していたら。
			if (result)
			{
				//カーソルが何もアイテムを持っていなかったら。
				if (m_grabed = nullptr)
				{
					//カーソルにアイテムを持たせる。
					m_grabed = std::make_unique<ItemStack>(result->GetItem(), result->GetNumber());
					//スロットを空にする。
					result.reset();
				}
				else {
					//マウスカーソルとスロットのアイテムを入れ替える。
					m_grabed.swap(result);
				}
			}
			//装備品が存在していなかったら。
			else 
			{
				//カーソルが何かアイテムを持っていたら。
				if (m_grabed != nullptr) {
					//インベントリーに1個だけアイテムを追加する。
					auto& slot = m_inventory.GetItem(slotNo);
					slot = std::make_unique<ItemStack>(m_grabed->GetItem(), 1);
					int num = m_grabed->GetNumber() - 1;
					//カーソルのアイテムが0より上なら。
					if (num > 0) {
						m_grabed->SetNumber(num);
					}
					//カーソルのアイテムが無くなったら。
					else {
						m_grabed.reset();
					}	
				}
				//カーソルが何もアイテムを持っていなかったら。
				else {
					//何も処理しない。
					return;
				}
			}
		}
	}
}