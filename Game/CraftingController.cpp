#include "stdafx.h"
#include "CraftingController.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace GUI::Controller{
	CraftingController::CraftingController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed ) :
		InventoryController( craftingSlots, grabed ), m_playerInventory( inventory ){

	}

	CraftingController::~CraftingController(){
		//GUIを閉じたときにクラフトテーブルのアイテムを返却する。
		for( auto& item : m_inventory ){
			m_playerInventory.AddItem( item );
		}
	}

	void CraftingController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		const unsigned resultSlot = 9;

		if( slotNo == resultSlot ){
			//完成品スロットをクリックしたときの動作。
			auto& result = m_inventory.GetItem( resultSlot );

			if( result == nullptr ){
				return;
			}
			if( m_grabed == nullptr ){
				//カーソルが空なら個数0の完成品と同じアイテムを持たせる。
				m_grabed = std::make_unique<ItemStack>( result->GetItem(), 0 );
			}
			if( m_grabed->GetID() != result->GetID() ){
				return;
			}

			//アイテム移動の処理
			const int grabNum = m_grabed->GetNumber();

			const int moveNum = min( 1, m_grabed->GetStackLimit() - grabNum );

			if( moveNum == 0 ){
				return;
			}

			m_grabed->SetNumber( grabNum + moveNum );
			result->SetNumber( result->GetNumber() - moveNum );

			if( result->GetNumber() == 0 ){
				m_inventory.SetItem( resultSlot, nullptr );
			}

			//完成品を取ったので、クラフトテーブルに置いてあるアイテムを消費する。
			for( int i = 0; i <= 8; i++ ){
				auto& item = m_inventory.GetItem( i );

				if( item ){
					item->SetNumber( item->GetNumber() - moveNum );

					if( item->GetNumber() == 0 ){
						m_inventory.SetItem( i, nullptr );
					}
				}
			}
			return;
		}

		//通常のクリック操作。
		InventoryController::OnClickSlot( event, slotNo );

		//クラフト結果を検索。
		auto& rm = RecipeManager::Instance();

		int itemArray[9];
		for( int i = 0; i < 9; i++ ){

			auto& item = m_inventory.GetItem( i );

			if( item ){
				itemArray[i] = item->GetID();
			} else{
				//nullをアイテム無しを表すenumに置き換える。
				itemArray[i] = enCube_None;
			}

		}

		auto result = rm.GetResult( itemArray );

		m_inventory.SetItem( 9, std::move( result ) );

	}
}