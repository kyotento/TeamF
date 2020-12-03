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
	class PlayerInventory : public RootNode{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();

		//! @brief GUIManager が呼ぶ描画関数。
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief 最前面に描画する関数。
		void DrawForeground() override;

		//! @return GUIのサイズ。
		CVector2 GetSize() const override;

	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
	};

}