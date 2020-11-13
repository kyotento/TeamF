//! @file
#pragma once
#include "Node.h"

class Inventory;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief インベントリの画面表示を行うクラス。
	//! @author Takayama
	class InventoryGUI : public Node{
	public:
		InventoryGUI(Inventory& inventory);

		~InventoryGUI();

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& scale ) override;

		//! @brief 最前面に描画する関数。
		void DrawForeground() override;

		//! @return GUIのサイズ。
		CVector2 GetSize() const override;

	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
	};

	//! @brief インベントリスロットごとのアイテム表示を行うクラス。
	//! @details クリックを感知してコントローラーへ処理を回すことも行う。
	//! @author Takayama
	class InventorySlot : public Node{
	public:
		InventorySlot(Inventory& inventory, Controller::InventoryController& controller,
					   unsigned slotNo , const CVector2& pos );
		~InventorySlot();

		//! @return GUIのサイズ。
		CVector2 GetSize() const override{
			return CVector2{ float(WIDTH), float(WIDTH) } * GetScale();
		}

		//! @brief GUIManager が呼ぶクリックイベント関数。
		void OnClick( GUI::Event::ClickEvent& event ) override;

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& scale ) override;

		//! @return インベントリのスロット番号。
		unsigned GetSlotNo() const{
			return m_slotNo;
		}

		//! @brief スロット1個の正方形の幅。
		static constexpr unsigned WIDTH = 36;

	private:
		const unsigned m_slotNo;
		Inventory& m_inventory;
		Controller::InventoryController& m_controller;
	};

}