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
	//スタック上限
	const int stackLimit = item->GetStackLimit();

	for( auto& slot : m_slotArray ){
		//空いているスロットがあればそこに入れて終了。
		if( !slot ){
			slot.swap( item );
			return;
		}

		//同じ種類のアイテムが入ってたら
		if( slot->GetID() == item->GetID() ){
			int tempNum = slot->GetNumber() + item->GetNumber();

			//アイテム溢れたら
			if( tempNum > stackLimit ){
				slot->SetNumber( stackLimit );
				item->SetNumber( tempNum - stackLimit );
			} else{
				//溢れなかったら
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

	//どっちにも何もなければ何もしない。
	if( !item && !slot ){
		return;
	}

	//スロットが空か、カーソルが空か、アイテムIDが異なる場合は、両者を入れ替える。
	if( !slot || !item || slot->GetID() != item->GetID() ){
		slot.swap( item );
		return;
	}

	//同じ種類のアイテムが入ってたら
	int tempNum = slot->GetNumber() + item->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//アイテム溢れたら
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		item->SetNumber( tempNum - stackLimit );
	} else{
		//溢れなかったら
		slot->SetNumber( tempNum );
		item.reset();
	}
}

void Inventory::RClickSlot( unsigned slotNo ){
	auto& slot = m_slotArray[slotNo];
	auto& item = m_grabedItem;

	//どっちにも何もなければ何もしない。
	if( !item && !slot ){
		return;
	}

	//カーソルが何も掴んでなくて、スロットにアイテムがある場合、半分だけ取る。
	if( !item && slot){
		int num = slot->GetNumber();
		int half = num / 2;

		item = std::make_unique<ItemStack>( slot->GetItem(), num - half );
		slot->SetNumber( half );
		return;
	}

	//カーソルにアイテムがあって、スロットにアイテムがない場合、1個だけ置く。
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

	//カーソルとスロットが同じアイテムなら1個だけ置く。
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

	//カーソルとスロットが違うアイテムなら両者を交換する。
	item.swap( slot );
}