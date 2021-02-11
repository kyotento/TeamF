#pragma once
#include "InventoryView.h"
#include "Inventory.h"

class FurnaceFire;

namespace GUI{

	class FurnaceInventory : public InventoryView{
	public:
		FurnaceInventory( Inventory& inventory, Inventory& furnaceInv, FurnaceFire& fire );

		~FurnaceInventory();

	private:
		std::unique_ptr<Controller::InventoryController> m_controller;
	};

	class FurnaceFireImg : public Node{
	public:
		FurnaceFireImg(FurnaceFire& fire, const CVector2& pos);

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @return GUIのサイズ。
		CVector2 GetSize() const override;

	private:
		FurnaceFire& m_fire;
		CSprite m_sprite;
	};

	class FurnaceArrowImg : public Node{
	public:
		FurnaceArrowImg( FurnaceFire& fire, const CVector2& pos );

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @return GUIのサイズ。
		CVector2 GetSize() const override;

	private:
		FurnaceFire& m_fire;
		CSprite m_sprite;
	};
}