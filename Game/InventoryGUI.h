//! @file
#pragma once
#include "Node.h"

class Inventory;

namespace GUI{

	class InventoryGUI : public Node{
	public:
		InventoryGUI(Inventory& inventory);
		~InventoryGUI();

		void Draw( const CVector2& pos, const CVector2& scale ) override;

		CVector2 GetSize() const override;

		void OnClick(GUI::Event::ClickEvent& event) override;

	private:
		Inventory& m_inventory;
		CSprite m_sprite;
	};

	class InventorySlot : public Node{
	public:
		InventorySlot(int slotNo , const CVector2& pos );
		~InventorySlot();

		CVector2 GetSize() const override{
			return CVector2{ float(WIDTH), float(WIDTH) } * GetScale();
		}

		void OnClick( GUI::Event::ClickEvent& event ) override;

		static constexpr int WIDTH = 36;
		const int slotNo;
	private:
	};

}