#include "stdafx.h"
#include "InventoryGUI.h"
#include "Inventory.h"
#include "ItemImage.h"
#include "ItemStack.h"
#include "ClickEvent.h"
#include "GUIManager.h"
#include "InventoryController.h"

namespace GUI{
	InventoryGUI::InventoryGUI( Inventory& inventory ) :
		m_inventory( inventory ){
		//画像をロード。
		m_sprite.Init( L"Resource/spriteData/KariInventory.dds" );

		//位置、ピボット、スケールを設定。
		auto& grEn = GetGraphicsEngine();
		const float bufW = grEn.GetFrameBuffer_W();
		const float bufH = grEn.GetFrameBuffer_H();

		SetPos( { bufW * 0.5f, bufH * 0.5f } );
		SetPivot( { 0.5f, 0.5f } );
		SetScale( { 1.5f, 1.5f } );

		//コントローラーを作成。
		m_controller = std::make_unique<Controller::InventoryController>( inventory );

		//スロットを充填。
		//ホットバー
		const CVector2 hotBarStart{ 14, 282 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 0,
													 hotBarStart, 9, 1 ) );
		//上段の9*3のアイテム入れるところ。
		const CVector2 upperStart{ 14, 166 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 9,
													 upperStart, 9, 3) );

		GUIManager::Instance().AddRoot( this );
	}

	InventoryGUI::~InventoryGUI(){
		GUIManager::Instance().RemoveRoot( this );
	}

	void InventoryGUI::Draw( const CVector2& pos, const CVector2& parentScale ){
		m_sprite.DrawScreenPos( pos, parentScale * GetScale(), GetPivot() );
	}

	void InventoryGUI::DrawForeground(){
		ItemStack* grab = m_inventory.GetGrabItem();
		if( grab ){
			//マウス座標の作成。
			CVector2 mouse = MouseCursor().GetMouseCursorPos();
			auto& grEn = GetGraphicsEngine();
			mouse.x *= grEn.GetFrameBuffer_W();
			mouse.y *= grEn.GetFrameBuffer_H();
			grab->Draw( mouse, GetScale() );
		}
	}

	CVector2 InventoryGUI::GetSize() const{
		CVector2 size{ static_cast<float>( m_sprite.GetWidth() ),
						static_cast<float>( m_sprite.GetHeight() ) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

	InventorySlots::InventorySlots(
		Inventory& inventory, Controller::InventoryController& controller,
		unsigned slotNoStart, const CVector2& posUpperLeft, unsigned widthNum, unsigned heightNum ) :

		m_slotNoStart( slotNoStart ), m_inventory( inventory ), m_controller( controller ),
		m_widthNum( widthNum ), m_heightNum( heightNum ){

		SetPos( posUpperLeft );
		SetPivot( { 0, 0 } );
	}

	InventorySlots::~InventorySlots(){}

	void InventorySlots::OnClick( GUI::Event::ClickEvent & event ){
		CVector2 pos = event.GetPos();

		int x = int( std::floorf( pos.x / (float( SLOT_WIDTH )) ) );
		int y = int( std::floorf( pos.y / (float( SLOT_WIDTH )) ) );
		//列と行からスロット番号を算出。
		m_controller.OnClickSlot( event, m_slotNoStart + x + y * m_widthNum );
	}

	void InventorySlots::Draw( const CVector2 & pos, const CVector2 & parentScale ){
		const int width = m_widthNum;
		const int height = m_heightNum;

		//アイテムのピボットぶんだけ開始位置をずらす。
		const CVector2 startPos = pos + (CVector2{ 0.5f, 0.5f } * SLOT_WIDTH * parentScale);

		//自分に含まれるすべてのスロットのアイテムを描画。
		for( int y = 0; y < height; y++ ){
			for( int x = 0; x < width; x++ ){
				//列と行からスロット番号を算出。
				ItemStack* itemStack = m_inventory.GetItem( m_slotNoStart + x + y * m_widthNum );
				if( itemStack ){
					CVector2 plusPos{ float(x * SLOT_WIDTH) , float(y * SLOT_WIDTH) };
					plusPos *= parentScale;
					itemStack->Draw( startPos + plusPos, parentScale * GetScale() );
				}
			}
		}
	}
}
