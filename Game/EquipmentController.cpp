#include "stdafx.h"
#include "EquipmentController.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace {
	const unsigned craftSlot = 4;
	const unsigned playerSlot = 35;
	const unsigned equipSlot = 5;
	int equipSlotList[4] = { enTool_Helmet,enTool_Plate,enTool_Leggings,enTool_Boots };
}

namespace GUI::Controller {
	EquipmentController::EquipmentController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

		//インベントリにアイテムを充填する。
		for (int i = 1; i < equipSlot; i++)
		{
			auto& pSlot = m_playerInventory.GetItem(i + playerSlot);
			if (m_playerInventory.GetNullableItem(i + playerSlot).GetID() != enCube_None
				&& pSlot) {
				auto item = std::make_unique<ItemStack>(Item::GetItem(pSlot.get()->GetID()), 1);
				m_inventory.SetItem(i+craftSlot,std::move(item));
			}
		}
	}

	EquipmentController::~EquipmentController() {
		//GUIを閉じたときにクラフトテーブルのアイテムを返却する。
		/*for (auto& item : m_inventory) {
			m_playerInventory.AddItem(item);
		}*/
	}

	void EquipmentController::OnMouseEvent(Event::MouseEvent & event, unsigned slotNo) {
		if( event.IsClick() == false ){
			return;
		}

		//クラフトスロットより上なら操作する。
		if (slotNo > craftSlot) {
			//装備スロットをクリックしたときの動作。
			//auto& result = m_playerInventory.GetItem(slotNo + playerSlot);
			auto& result = m_inventory.GetItem(slotNo);
			//スロットに装備品が既に存在していたら。
			if (result != nullptr)
			{
				//カーソルが何もアイテムを持っていなかったら。
				if (m_grabed == nullptr)
				{
					//カーソルにアイテムを持たせる。
					m_grabed = std::make_unique<ItemStack>(result->GetItem(), result->GetNumber());
					//スロットを空にする。
					result.reset();
					//プレイヤーインベントリも空にする。
					m_playerInventory.GetItem(slotNo + playerSlot- craftSlot).reset();
					return;
				}
				//カーソルがアイテムを持っていたら。
				else {
					//スロットとマウスカーソルのアイテムが違っていたら。
					if (m_grabed->GetID() != result->GetID())
					{
						if (!DetermineItemPlaced(slotNo, m_grabed->GetToolID()))
						{
							return;
						}
						//マウスカーソルのアイテムが一個だけだったら。
						if (m_grabed->GetNumber() == 1)
						{

							//マウスカーソルとスロットのアイテムを入れ替える。
							m_grabed.swap(result);
							auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
							//プレイヤースロットのアイテムも変える。
							//pSlot = std::make_unique<ItemStack>(result->GetItem());
							pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);

							
						}
						//TODO 一個じゃない時ってどうするんですかね。そもそも装備って他のアイテムみたいに複数個一気に持てないと思うんですけど(名推理)。
						else {

						}
						return;
					}
					//スロットとマウスカーソルのアイテムが一緒だったら。
					else {
						//同じ種類のアイテムが入ってたら。
						int tempNum = result->GetNumber() + m_grabed->GetNumber();
						const int stackLimit = result->GetStackLimit();
						auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
						//アイテム溢れたら。
						if (tempNum > stackLimit) {
							m_grabed->SetNumber(stackLimit);
							result->SetNumber(tempNum - stackLimit);
							//プレイヤースロットのアイテムも変える。
							pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);
						}
						else {
							//溢れなかったら。
							m_grabed->SetNumber(tempNum);
							result.reset();
							pSlot.reset();
						}
						return;
					}
				}
			}
			//スロットに装備品が存在していなかったら。
			else 
			{
				//カーソルが何かアイテムを持っていたら。
				if (m_grabed != nullptr) {
					//カーソルが該当のアイテムじゃなかったら何もしない。
					if (!DetermineItemPlaced(slotNo, m_grabed->GetToolID()))
					{
						return;
					}
					//インベントリーに1個だけアイテムを追加する。
					auto& slot = m_inventory.GetItem(slotNo);
			
					slot = std::make_unique<ItemStack>(m_grabed->GetItem(), 1);
					auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
					//プレイヤースロットのアイテムも変える。
					pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);
					int num = m_grabed->GetNumber() - 1;
					//カーソルのアイテムが0より上なら。
					if (num > 0) {
						m_grabed->SetNumber(num);
					}
					//カーソルのアイテムが無くなったら。
					else {
						m_grabed.reset();
					}
					return;
				}
				//カーソルが何もアイテムを持っていなかったら。
				else {
					//何も処理しない。
					return;
				}
			}
		}
	}

	bool EquipmentController::DetermineItemPlaced(int slots,const int itemId)
	{
		//強制装備
		if (GetKeyInput(VK_TAB)) {
			return true;
		}

		slots = slots - craftSlot;
		if (itemId == equipSlotList[slots-1])
		{
			return true;
		}
		else {
			return false;
		}
	}
}

