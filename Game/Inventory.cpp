#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "ItemType.h"

Inventory::Inventory( unsigned size ){
	m_slotArray.resize( size );
}

Inventory::~Inventory(){}

void Inventory::SetItem( unsigned slotNo, std::unique_ptr<ItemStack> item ){
	m_slotArray[slotNo] = std::move( item );
}

NullableItemStack & Inventory::GetNullableItem( unsigned slotNo ){
	if( m_slotArray[slotNo] ){
		return *m_slotArray[slotNo];
	} else{
		return NullableItemStack::Instance();
	}
}

void Inventory::AddItem( std::unique_ptr<ItemStack>& item ){
	if( item == nullptr ){
		return;
	}

	//スタック上限
	const int stackLimit = item->GetStackLimit();

	for( auto& slot : m_slotArray ){
		//空いているスロットがあればそこに入れる。
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

void Inventory::LClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor ){
	auto& slot = m_slotArray[slotNo];

	//どっちにも何もなければ何もしない。
	if( !cursor && !slot ){
		return;
	}

	//スロットが空か、カーソルが空か、アイテムIDが異なる場合は、両者を入れ替える。
	if( !slot || !cursor || slot->GetID() != cursor->GetID() ){
		slot.swap( cursor );
		return;
	}

	//同じ種類のアイテムが入ってたら
	int tempNum = slot->GetNumber() + cursor->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//アイテム溢れたら
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		cursor->SetNumber( tempNum - stackLimit );
	} else{
		//溢れなかったら
		slot->SetNumber( tempNum );
		cursor.reset();
	}
}

void Inventory::RClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor ){
	auto& slot = m_slotArray[slotNo];

	//どっちにも何もなければ何もしない。
	if( !cursor && !slot ){
		return;
	}

	//カーソルが何も掴んでなくて、スロットにアイテムがある場合、半分だけ取る。
	if( !cursor && slot ){
		int num = slot->GetNumber();
		int half = num / 2;

		cursor = std::make_unique<ItemStack>( slot->GetItem(), num - half );

		if( half == 0 ){
			slot.reset();
		} else{
			slot->SetNumber( half );
		}
		return;
	}

	//カーソルにアイテムがあって、スロットにアイテムがない場合、1個だけ置く。
	if( cursor && !slot ){
		slot = std::make_unique<ItemStack>( cursor->GetItem(), 1 );
		int num = cursor->GetNumber() - 1;
		if( num > 0 ){
			cursor->SetNumber( num );
		} else{
			cursor.reset();
		}
		return;
	}

	//カーソルとスロットが同じアイテムなら1個だけ置く。
	if( cursor->GetID() == slot->GetID() ){
		if( slot->GetNumber() == slot->GetStackLimit() ){
			return;
		}

		int num = cursor->GetNumber() - 1;
		if( num > 0 ){
			cursor->SetNumber( num );
		} else{
			cursor.reset();
		}
		slot->SetNumber( slot->GetNumber() + 1 );
		return;
	}

	//カーソルとスロットが違うアイテムなら両者を交換する。
	cursor.swap( slot );
}