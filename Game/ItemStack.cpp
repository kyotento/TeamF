#include "stdafx.h"
#include "ItemStack.h"
#include "InventorySlot.h"

std::unique_ptr<ItemStack> ItemStack::TakeItem( std::unique_ptr<ItemStack>& item, int num ){
	if( item->GetNumber() <= num ){
		return std::move( item );
	}
	item->SetNumber( item->GetNumber() - num );
	return std::make_unique<ItemStack>( item->GetItem(), num );
}

void ItemStack::Draw( const CVector2 & pos, const CVector2 & parentScale ){
	m_item.Draw( pos, parentScale );

	if( m_number == 1 ){
		return;
	}

	wchar_t numText[3];
	swprintf_s( numText, L"%d", m_number );

	const unsigned slotW = GUI::InventorySlots::SLOT_WIDTH;
	const CVector2 addPos = CVector2{ slotW * 0.4f, slotW * 0.4f } * parentScale;
	m_font.DrawScreenPos( numText, pos + addPos, CVector4::White(), CVector2::One() * 0.5f, { 1.0f, 1.0f } );
}
