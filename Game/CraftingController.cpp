#include "stdafx.h"
#include "CraftingController.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace GUI::Controller{
	CraftingController::CraftingController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed ) :
		InventoryController(craftingSlots, grabed), m_playerInventory( inventory ){

	}

	CraftingController::~CraftingController(){
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		for( auto& item : m_inventory ){
			m_playerInventory.AddItem( item );
		}
	}

	void CraftingController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		if( slotNo == 9 ){
			
		} else{
			//�ʏ�̃N���b�N����B
			InventoryController::OnClickSlot( event, slotNo );

			//�N���t�g���ʂ������B
			auto& rm = RecipeManager::Instance();

			int itemArray[9];
			for( int i = 0; i < 9; i++ ){

				auto& item = m_inventory.GetItem( i );

				if( item ){
					itemArray[i] = item->GetID();
				} else{
					itemArray[i] = enCube_None;
				}

			}

			auto result = rm.GetResult( itemArray );

			m_inventory.SetItem( 9, std::move(result) );
		}
	}
}