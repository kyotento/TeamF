//! @file
#pragma once
#include "Node.h"

class Inventory;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief インベントリスロットごとのアイテム表示を行うクラス。
		//! @details クリックを感知してコントローラーへ処理を回すことも行う。
		//! @author Takayama
	class InventorySlots : public Node{
	public:
		InventorySlots( Inventory& inventory, Controller::InventoryController& controller,
						unsigned slotNoStart, const CVector2& posUpperLeft, unsigned widthNum, unsigned heightNum );
		~InventorySlots();

		//! @return GUIのサイズ。
		CVector2 GetSize() const override{
			return CVector2{ float( m_widthNum * SLOT_WIDTH ), float( m_heightNum * SLOT_WIDTH ) } *GetScale();
		}

		//! @brief GUIManager が呼ぶマウスが乗った時イベント関数。
		void OnMouseEvent( GUI::Event::MouseEvent& event ) override;

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief スロット1個の正方形の幅。
		static constexpr unsigned SLOT_WIDTH = 36;

	private:
		const unsigned m_slotNoStart;
		const unsigned m_widthNum;
		const unsigned m_heightNum;

		Inventory& m_inventory;
		Controller::InventoryController& m_controller;
	};

}