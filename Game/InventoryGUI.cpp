#include "stdafx.h"
#include "InventoryGUI.h"
#include "GUIManager.h"
#include "ClickEvent.h"

namespace GUI{
	InventoryGUI::InventoryGUI( Inventory& inventory ) : m_inventory( inventory ){
		m_sprite.Init( L"Resource/spriteData/KariInventory.dds" );

		auto& grEn = GetGraphicsEngine();
		const float bufW = grEn.GetFrameBuffer_W();
		const float bufH = grEn.GetFrameBuffer_H();

		SetPos( { bufW * 0.5f, bufH * 0.5f } );
		SetPivot( { 0.5f, 0.5f } );

		SetScale( {1.5f, 1.5f} );

		//スロットを充填。
		const CVector2 slotStart{ 14, 166 };
		for( int x = 0; x < 9; x++ ){
			for( int y = 0; y < 3; y++ ){

				CVector2 pos = slotStart;
				pos.x += x * InventorySlot::WIDTH;
				pos.y += y * InventorySlot::WIDTH;

				AddChilde( std::make_unique<InventorySlot>(x + y*9 , pos ) );

			}
		}

		GUIManager::Instance().AddRoot( this );
	}

	InventoryGUI::~InventoryGUI(){
		GUIManager::Instance().RemoveRoot( this );
	}

	void InventoryGUI::Draw( const CVector2& pos, const CVector2& scale ){
		m_sprite.DrawScreenPos( pos , scale, GetPivot());
	}

	CVector2 InventoryGUI::GetSize() const{
		CVector2 size{ static_cast<float>(m_sprite.GetWidth()),
						static_cast<float>(m_sprite.GetHeight()) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

	void InventoryGUI::OnClick( GUI::Event::ClickEvent & event ){

		if( event.GetClickType() == event.LEFT ){
			OutputDebugStringA( "ひだりくりっく\n" );
		}
		if( event.GetClickType() == event.RIGHT ){
			OutputDebugStringA( "みぎくりっく\n" );
		}
	}

	InventorySlot::InventorySlot( int slotNo, const CVector2 & pos ) : slotNo(slotNo){
		SetPos( pos );
	}

	InventorySlot::~InventorySlot(){}

	void InventorySlot::OnClick( GUI::Event::ClickEvent & event ){
		std::stringstream str;
		str << slotNo << "番スロット\n";
		OutputDebugStringA( str.str().c_str());
	}
}
