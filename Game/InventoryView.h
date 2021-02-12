#pragma once
#include "Node.h"

class Inventory;
class ItemStack;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief Inventory系GUIの基底クラス
	//! @author Takayama
	class InventoryView : public RootNode{
	public:
		InventoryView( Inventory& playerInventory, const wchar_t* spritePath, const CVector4& m_shiftColor = CVector4::One());

		virtual ~InventoryView();

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief 最前面に描画する関数。
		void DrawForeground() override;

		//! @return GUIのサイズ。
		CVector2 GetSize() const override;

	protected:
		//掴んでいるアイテム
		std::unique_ptr<ItemStack> m_grabed;
	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
		CVector4 m_shiftColor = CVector4::One();
	};

}