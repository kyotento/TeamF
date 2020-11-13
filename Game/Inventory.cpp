#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "ItemType.h"

Inventory::Inventory(unsigned size){
	m_slotArray.resize( size );

	auto item = std::make_unique<ItemStack>( Item::GetItem( enItem_GoldenPickaxe) );
	auto item2 = std::make_unique<ItemStack>( Item::GetItem( enItem_DiamondHoe ) );
	AddItem( item );
	AddItem( item2 );
}

Inventory::~Inventory(){}

void Inventory::SetItem( unsigned slotNo, std::unique_ptr<ItemStack> item ){
	m_slotArray[slotNo] = std::move( item );
}

void Inventory::SetGrabItem( std::unique_ptr<ItemStack> item ){
	m_grabedItem = std::move( item );
}

void Inventory::AddItem( std::unique_ptr<ItemStack>& item ){
	//�X�^�b�N���
	const int stackLimit = item->GetStackLimit();

	for( auto& slot : m_slotArray ){
		//�󂢂Ă���X���b�g������΂����ɓ���ďI���B
		if( !slot ){
			slot.swap( item );
			return;
		}

		//������ނ̃A�C�e���������Ă���
		if( slot->GetID() == item->GetID() ){
			int tempNum = slot->GetNumber() + item->GetNumber();

			//�A�C�e����ꂽ��
			if( tempNum > stackLimit ){
				slot->SetNumber( stackLimit );
				item->SetNumber( tempNum - stackLimit );
			} else{
				//���Ȃ�������
				slot->SetNumber( tempNum );
				item.reset();
				return;
			}
		}
	}
}

void Inventory::LClickSlot( unsigned slotNo){
	auto& slot = m_slotArray[slotNo];
	auto& item = m_grabedItem;

	//�ǂ����ɂ������Ȃ���Ή������Ȃ��B
	if( !item && !slot ){
		return;
	}

	//�X���b�g���󂩁A�J�[�\�����󂩁A�A�C�e��ID���قȂ�ꍇ�́A���҂����ւ���B
	if( !slot || !item || slot->GetID() != item->GetID() ){
		slot.swap( item );
		return;
	}

	//������ނ̃A�C�e���������Ă���
	int tempNum = slot->GetNumber() + item->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//�A�C�e����ꂽ��
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		item->SetNumber( tempNum - stackLimit );
	} else{
		//���Ȃ�������
		slot->SetNumber( tempNum );
		item.reset();
	}
}

void Inventory::RClickSlot( unsigned slotNo ){
	auto& slot = m_slotArray[slotNo];
	auto& item = m_grabedItem;

	//�ǂ����ɂ������Ȃ���Ή������Ȃ��B
	if( !item && !slot ){
		return;
	}

	//�J�[�\���������͂�łȂ��āA�X���b�g�ɃA�C�e��������ꍇ�A�����������B
	if( !item && slot){
		int num = slot->GetNumber();
		int half = num / 2;

		item = std::make_unique<ItemStack>( slot->GetItem(), num - half );
		slot->SetNumber( half );
		return;
	}

	//�J�[�\���ɃA�C�e���������āA�X���b�g�ɃA�C�e�����Ȃ��ꍇ�A1�����u���B
	if( item && !slot ){
		int num = item->GetNumber() - 1;
		if( num > 0 ){
			item->SetNumber( num );
		} else{
			item.reset();
		}
		slot = std::make_unique<ItemStack>( item->GetItem(), 1 );
		return;
	}

	//�J�[�\���ƃX���b�g�������A�C�e���Ȃ�1�����u���B
	if( item->GetID() == slot->GetID() ){
		if( slot->GetNumber() == slot->GetStackLimit() ){
			return;
		}

		int num = item->GetNumber() - 1;
		if( num > 0 ){
			item->SetNumber( num );
		} else{
			item.reset();
		}
		slot->SetNumber( slot->GetNumber() + 1 );
		return;
	}

	//�J�[�\���ƃX���b�g���Ⴄ�A�C�e���Ȃ痼�҂���������B
	item.swap( slot );
}