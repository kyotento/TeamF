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
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		for( auto& item : m_inventory ){
			m_playerInventory.AddItem( item );
		}
	}

	void CraftingController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		const unsigned resultSlot = 9;

		if( slotNo == resultSlot ){
			//�����i�X���b�g���N���b�N�����Ƃ��̓���B
			auto& result = m_inventory.GetItem( resultSlot );

			if( result == nullptr ){
				return;
			}
			if( m_grabed == nullptr ){
				//�J�[�\������Ȃ��0�̊����i�Ɠ����A�C�e������������B
				m_grabed = std::make_unique<ItemStack>( result->GetItem(), 0 );
			}
			if( m_grabed->GetID() != result->GetID() ){
				return;
			}

			//�A�C�e���ړ��̏���
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

			//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
			for( int i = 0; i <= 8; i++ ){
				auto& item = m_inventory.GetItem( i );

				if( item ){
					item->SetNumber( item->GetNumber() - moveNum );

					if( item->GetNumber() == 0 ){
						m_inventory.SetItem( i, nullptr );
					}
				}
			}
		}

		if( slotNo != resultSlot ){
			//�ʏ�̃N���b�N����B
			InventoryController::OnClickSlot( event, slotNo );
		}

		//�N���t�g���ʂ������B���ʕ��X���b�g���X�V����B
		auto& rm = RecipeManager::Instance();

		int itemArray[9];
		for( int i = 0; i < 9; i++ ){

			auto& item = m_inventory.GetItem( i );

			if( item ){
				itemArray[i] = item->GetID();
			} else{
				//null���A�C�e��������\��enum�ɒu��������B
				itemArray[i] = enCube_None;
			}

		}

		auto result = rm.GetResult( itemArray );

		m_inventory.SetItem( 9, std::move( result ) );
	}
}