#include "stdafx.h"
#include "CraftingController.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"
#include "Player.h"
#include "ZeroableStack.h"

namespace GUI::Controller{
	static const int resultSlot = 9;

	CraftingController::CraftingController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed ) :
		InventoryController( craftingSlots, grabed ), m_playerInventory( inventory ){

	}

	CraftingController::~CraftingController(){
		int number = 0;
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		for (auto& item : m_inventory) {
			if (number == 9)
			{
				return;
			}
			//�A�C�e�����v���C���[�C���x���g���ɕԋp�ł��Ȃ�������B
			if (m_inventory.GetNullableItem(number).GetID() != enCube_None && !m_playerInventory.AddItem(item))
			{
				//�h���b�v�A�C�e���𐶐�����B
				auto player = FindGO<Player>();
				if (player != nullptr)
				{
					player->CreateFrontDropItem(item);
				}

			}
			number++;
		}
	}

	void CraftingController::OnMouseEvent( Event::MouseEvent & event, unsigned slotNo ){

		if( slotNo == resultSlot ){
			if( event.IsClick() == false ){
				return;
			}
			if( event.IsPressShift() ){
				ShiftCraft( std::move(m_inventory.GetItem( resultSlot )) );
				return;
			}

			//�����i�X���b�g���N���b�N�����Ƃ��̓���B
			ZeroableStack result( m_inventory.GetItem( resultSlot ) );
			ZeroableStack grab( m_grabed );

			int moveNum = grab.GetCanTakeFrom( result );

			if( moveNum == 0 || moveNum < result.GetNumber() ){
				return;
			}

			result.AddNumber( -moveNum );
			grab.AddNumber( +moveNum );

			//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
			for( int i = 0; i < 9; i++ ){
				ZeroableStack item( m_inventory.GetItem( i ) );
				item.AddNumber( -1 );
			}
		}

		if( slotNo != resultSlot ){
			//�ʏ�̃N���b�N����B
			InventoryController::OnMouseEvent( event, slotNo );
		}

		//�N���t�g���ʂ������B���ʕ��X���b�g���X�V����B
		int itemArray[9];
		for( int i = 0; i < 9; i++ ){
			ZeroableStack item(m_inventory.GetItem( i ));
			itemArray[i] = item.GetID();
		}

		auto result = RecipeManager::Instance().GetResult( itemArray );
		m_inventory.SetItem( resultSlot, std::move( result ) );
	}

	void CraftingController::ShiftCraft( std::unique_ptr<ItemStack>&& result ){
		if( result == nullptr ){
			return;
		}

		m_playerInventory.AddItem( result );

		if( result ){
			m_inventory.SetItem( resultSlot, std::move(result) );
			return;
		}

		//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
		for( int i = 0; i < 9; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			item.AddNumber( -1 );
		}

		//�N���t�g���ʂ������B���ʕ����X�V����B
		int itemArray[9];
		for( int i = 0; i < 9; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			itemArray[i] = item.GetID();
		}

		auto nextResult = RecipeManager::Instance().GetResult( itemArray );
		ShiftCraft( std::move( nextResult ) );
	}
}