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
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		int number = 0;
		for (auto& item : m_inventory) 
		{	//�N���t�g�X���b�g�̂Ƃ��낾���A�C�e����ԋp����B
			if (number <= resultSlot - 1) {
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

			//�����i�X���b�g���N���b�N�����Ƃ��̓���B
			ZeroableStack result(m_inventory.GetItem(resultSlot));
			ZeroableStack grab(m_grabed);

			int moveNum = grab.GetCanTakeFrom( result );

			if( moveNum == 0 || moveNum < result.GetNumber() ){
				return;
			}
			result.AddNumber( -moveNum );
			grab.AddNumber( +moveNum );

			//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
			for (int i = 0; i < 4; i++) {
				ZeroableStack item( m_inventory.GetItem(i));
				item.AddNumber( -1 );
			}
		}

		if (slotNo < resultSlot) {
			//�ʏ�̃N���b�N����B
			InventoryController::OnMouseEvent(event, slotNo);
		}

		//�N���t�g���ʂ������B���ʕ��X���b�g���X�V����B
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

		//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
		for( int i = 0; i < 4; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			item.AddNumber( -1 );
		}

		//�N���t�g���ʂ������B���ʕ����X�V����B
		int itemArray[4];
		for( int i = 0; i < 4; i++ ){
			ZeroableStack item( m_inventory.GetItem( i ) );
			itemArray[i] = item.GetID();
		}

		auto nextResult = RecipeManager::Instance().GetResult( itemArray );
		ShiftCraft( std::move( nextResult ) );
	}
}