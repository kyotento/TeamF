#include "stdafx.h"
#include "InventoryView.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "GUIManager.h"
#include "InventoryController.h"
#include "InventorySlot.h"

namespace GUI{

	InventoryView::InventoryView( Inventory& inventory, const wchar_t* spritePath, const CVector4& shiftColor) :
		m_inventory( inventory ), m_shiftColor(shiftColor){
		//�摜�����[�h�B
		m_sprite.Init( spritePath );

		//�ʒu�A�s�{�b�g�A�X�P�[����ݒ�B
		auto& grEn = GetGraphicsEngine();
		const float bufW = grEn.GetFrameBuffer_W();
		const float bufH = grEn.GetFrameBuffer_H();

		SetPos( { bufW * 0.5f, bufH * 0.5f } );
		SetPivot( { 0.5f, 0.5f } );
		SetScale( { 1.5f, 1.5f } );

		//�R���g���[���[���쐬�B
		m_controller = std::make_unique<Controller::InventoryController>( inventory, m_grabed );

		//�X���b�g���[�U�B
		//�z�b�g�o�[
		const CVector2 hotBarStart{ 14, 282 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 0,
													 hotBarStart, 9, 1 ) );
		//��i��9*3�̃A�C�e�������Ƃ���B
		const CVector2 upperStart{ 14, 166 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 9,
													 upperStart, 9, 3 ) );
	}

	InventoryView::~InventoryView(){
		m_inventory.AddItem( m_grabed );
	}

	void InventoryView::Draw( const CVector2& pos, const CVector2& parentScale ){
		CVector4 color = CVector4::One();
		if (GetKeyInput(VK_SHIFT)) {
			color = m_shiftColor;
		}
		m_sprite.DrawScreenPos( pos, parentScale * GetScale(), GetPivot(), 0.0f, color );
	}

	void InventoryView::DrawForeground(){
		if( m_grabed ){
			//�}�E�X���W�̍쐬�B
			CVector2 mouse = MouseCursor().GetMouseCursorPos();
			auto& grEn = GetGraphicsEngine();
			mouse.x *= grEn.GetFrameBuffer_W();
			mouse.y *= grEn.GetFrameBuffer_H();
			m_grabed->Draw( mouse, GetScale() );
		}
	}

	CVector2 InventoryView::GetSize() const{
		CVector2 size{ static_cast<float>( m_sprite.GetWidth() ),
						static_cast<float>( m_sprite.GetHeight() ) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

}