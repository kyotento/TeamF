#include "stdafx.h"
#include "FurnaceController.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "RecipeManager.h"
#include "ZeroableStack.h"

namespace GUI::Controller{
	FurnaceController::FurnaceController(
		Inventory & furnaceSlot, std::unique_ptr<ItemStack>& grabed, FurnaceFire& fire ) :
		InventoryController( furnaceSlot, grabed ), m_fire( fire ){

	}

	void FurnaceController::OnMouseEvent( Event::MouseEvent & event, unsigned slotNo ){
		if( event.IsClick() == false ){
			return;
		}

		ZeroableStack result( m_inventory.GetItem( Furnace::RESULT ) );
		ZeroableStack fuel( m_inventory.GetItem( Furnace::FUEL ) );
		ZeroableStack source( m_inventory.GetItem( Furnace::SOURCE ) );
		ZeroableStack grab( m_grabed );

		//燃料スロットに燃料以外は置かせない。
		if( slotNo == Furnace::FUEL && m_grabed && m_grabed->IsFuel() == false ){
			return;
		}

		//完成品スロットからは取るだけ。
		if( slotNo == Furnace::RESULT ){
			if( result == false ){
				return;
			}

			if(m_othorCtrl && event.IsPressShift() ){
				m_othorCtrl->AddItem( result.GetStack() );
			} else{
				grab.TakeFrom( result, INT_MAX );
			}

			//火がついていたらここで終わり。
			if( m_fire.isFire() ){
				return;
			}

			if( source == false ){
				return;
			}

			//レシピを参照して、燃やせるものだった場合、燃焼のためのGameObjectを生成する。
			auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

			if( recipeResult == nullptr ){
				return;
			}

			//完成品スロットに邪魔な物が入っていると燃やせない。
			if( result ){
				if( recipeResult->GetID() != result.GetID() ){
					return;
				}
				if( result.GetNumber() == result.GetItem()->GetStackLimit() ){
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

		if( source == false || fuel == false ){
			return;
		}

		//レシピを参照して、燃やせるものだった場合、燃焼のためのGameObjectを生成する。
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

		if( recipeResult == nullptr ){
			return;
		}

		//完成品スロットに邪魔な物が入っていると燃やせない。
		if( result ){
			if( recipeResult->GetID() != result.GetID() ){
				return;
			}
			if( result.GetNumber() == result.GetItem()->GetStackLimit() ){
				return;
			}
		}

		//火をつける。
		m_fire.setFire();
	}

	void FurnaceController::AddItem( std::unique_ptr<ItemStack>& item ){
		if( item == nullptr ){
			return;
		}
		ZeroableStack added( item );
		ZeroableStack fuel( m_inventory.GetItem( Furnace::FUEL ) );
		ZeroableStack source( m_inventory.GetItem( Furnace::SOURCE ) );

		if( item->IsFuel() ){
			fuel.TakeFrom( added, INT_MAX );
		} else{
			source.TakeFrom( added, INT_MAX );
		}

		//火がついていたらここで終わり。
		if( m_fire.isFire() ){
			return;
		}

		if( source == false || fuel == false ){
			return;
		}

		//レシピを参照して、燃やせるものだった場合、燃焼のためのGameObjectを生成する。
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

		if( recipeResult == nullptr ){
			return;
		}

		//完成品スロットに邪魔な物が入っていると燃やせない。
		auto& result = m_inventory.GetItem( Furnace::RESULT );
		if( result ){
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