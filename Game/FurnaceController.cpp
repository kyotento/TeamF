#include "stdafx.h"
#include "FurnaceController.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "RecipeManager.h"

namespace GUI::Controller{
	FurnaceController::FurnaceController(
		Inventory & furnaceSlot, std::unique_ptr<ItemStack>& grabed, FurnaceFire& fire ) :
		InventoryController( furnaceSlot, grabed ), m_fire(fire){

	}

	void FurnaceController::OnMouseEvent( Event::MouseEvent & event, unsigned slotNo ){
		auto& result = m_inventory.GetItem( Furnace::RESULT );
		auto& fuel = m_inventory.GetItem( Furnace::FUEL );
		auto& source = m_inventory.GetItem( Furnace::SOURCE );

		//燃料スロットに燃料以外は置かせない。
		if( slotNo == Furnace::FUEL && m_grabed && m_grabed->IsFuel() == false ){
			std::stringstream ss;
			ss << m_grabed->GetID() << "\n";
			OutputDebugStringA( ss.str().c_str() );
			return;
		}

		//完成品スロットからは取るだけ。
		if( slotNo == Furnace::RESULT){
			if( result == nullptr ){
				return;
			}

			//カーソルが空なら個数0の完成品と同じアイテムを持たせる。
			if( m_grabed == nullptr ){
				m_grabed = std::make_unique<ItemStack>( result->GetItem(), 0 );
			}

			if( m_grabed->GetID() == result->GetID() ){
				//完成品スロットから手持ちにアイテム移動。
				int total = m_grabed->GetNumber() + result->GetNumber();
				m_grabed->SetNumber( min( total, m_grabed->GetStackLimit() ) );

				//全体から手持ちを引くと、完成品スロットの個数になる。
				result->SetNumber( total - m_grabed->GetNumber() );
				if( result->GetNumber() <= 0 ){
					result.reset();
				}
			}
			if (source == nullptr)
			{
				return;
			}
			//レシピを参照して、燃やせるものだった場合、燃焼のためのGameObjectを生成する。
			auto recipeResult = RecipeManager::Instance().GetFurnaceResult(source->GetID());

			if (recipeResult == nullptr) {
				return;
			}

			//完成品スロットに邪魔な物が入っていると燃やせない。
			if (result) {
				if (recipeResult->GetID() != result->GetID()) {
					return;
				}
				if (result->GetNumber() == result->GetStackLimit()) {
					return;
				}
			}

			//火をつける。
			m_fire.setFire();

			return;
		}

		//通常のクリック操作。
		InventoryController::OnMouseEvent( event, slotNo );

		//火がついていたらここで終わり。
		if( m_fire.isFire() ){
			if( slotNo == Furnace::SOURCE ){
				m_fire.SourceChange();
			}
			return;
		}

		if( source == nullptr || fuel == nullptr ){
			return;
		}

		//レシピを参照して、燃やせるものだった場合、燃焼のためのGameObjectを生成する。
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult(source->GetID());

		if( recipeResult == nullptr ){
			return;
		}

		//完成品スロットに邪魔な物が入っていると燃やせない。
		if(result){
			if( recipeResult->GetID() != result->GetID() ){
				return;
			}
			if( result->GetNumber() == result->GetStackLimit() ){
				return;
			}
		}

		//火をつける。
		m_fire.setFire();
	}
}