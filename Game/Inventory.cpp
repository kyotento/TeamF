#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemStack.h"

Inventory::Inventory(unsigned size){
	m_slotArray.resize( size );
}

Inventory::~Inventory(){}

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

void Inventory::LClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& item ){
	auto& slot = m_slotArray[slotNo];

	//スロットが空か、アイテムIDが異なる場合は、両者を入れ替える。
	if( !slot || slot->GetID() != item->GetID() ){
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

void Inventory::RClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& item ){
	auto& slot = m_slotArray[slotNo];

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

//void Inventory::PostRender()
//{
//	wchar_t output[256];
//	swprintf_s(output, L"  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n",m_inventoryList[0]->s_item->GetItemName(),m_inventoryList[0]->s_number, m_inventoryList[1]->s_item->GetItemName(), m_inventoryList[1]->s_number, m_inventoryList[2]->s_item->GetItemName(), m_inventoryList[2]->s_number, m_inventoryList[3]->s_item->GetItemName(), m_inventoryList[3]->s_number,
//		m_inventoryList[4]->s_item->GetItemName(), m_inventoryList[4]->s_number, m_inventoryList[5]->s_item->GetItemName(), m_inventoryList[5]->s_number, m_inventoryList[6]->s_item->GetItemName(), m_inventoryList[6]->s_number, m_inventoryList[7]->s_item->GetItemName(), m_inventoryList[7]->s_number, m_inventoryList[8]->s_item->GetItemName(), m_inventoryList[8]->s_number);
//	m_font.DrawScreenPos(output, { 500.0f,300.0f }, CVector4::White(), { 0.6f,0.6f },
//		CVector2::Zero(),
//		0.0f,
//		DirectX::SpriteEffects_None,
//		0.7f
//	);
//}