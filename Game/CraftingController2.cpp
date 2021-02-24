#include "stdafx.h"
#include "CraftingController2.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"
#include "DropItem.h"
#include "Block.h"
#include "Player.h"
#include "ZeroableStack.h"

namespace GUI::Controller {
	static const int resultSlot = 4;

	CraftingController2::CraftingController2(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

	}

	CraftingController2::~CraftingController2() {
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

	void CraftingController2::OnMouseEvent(Event::MouseEvent & event, unsigned slotNo) {

		if (slotNo == resultSlot) {
			if( event.IsClick() == false ){
				return;
			}
			if( event.IsPressShift() ){
				ShiftCraft( std::move( m_inventory.GetItem( resultSlot ) ) );
				return;
			}

			//完成品スロットをクリックしたときの動作。
			ZeroableStack result(m_inventory.GetItem(resultSlot));
			ZeroableStack grab(m_grabed);

			int moveNum = grab.GetCanTakeFrom( result );

			if( moveNum == 0 || moveNum < result.GetNumber() ){
				return;
			}
			result.AddNumber( -moveNum );
			grab.AddNumber( +moveNum );

			//完成品を取ったので、クラフトテーブルに置いてあるアイテムを消費する。
			for (int i = 0; i < 4; i++) {
				ZeroableStack item( m_inventory.GetItem(i));
				item.AddNumber( -1 );
			}
		}

		if (slotNo < resultSlot) {
			//通常のクリック操作。
			InventoryController::OnMouseEvent(event, slotNo);
		}

		//クラフト結果を検索。成果物スロットを更新する。
		int itemArray[4];
		for( int i = 0; i < 4; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			itemArray[i] = item.GetID();
		}

		auto result = RecipeManager::Instance().GetResult( itemArray );
		m_inventory.SetItem( resultSlot, std::move( result ) );
	}

	void CraftingController2::ShiftCraft( std::unique_ptr<ItemStack>&& result ){
		if( result == nullptr ){
			return;
		}

		m_playerInventory.AddItem( result );

		if( result ){
			m_inventory.SetItem( resultSlot, std::move( result ) );
			return;
		}

		//完成品を取ったので、クラフトテーブルに置いてあるアイテムを消費する。
		for( int i = 0; i < 4; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			item.AddNumber( -1 );
		}

		//クラフト結果を検索。成果物を更新する。
		int itemArray[4];
		for( int i = 0; i < 4; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			itemArray[i] = item.GetID();
		}

		auto nextResult = RecipeManager::Instance().GetResult( itemArray );
		ShiftCraft( std::move( nextResult ) );
	}
}