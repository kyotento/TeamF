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

		//�R���X���b�g�ɔR���ȊO�͒u�����Ȃ��B
		if( slotNo == Furnace::FUEL && m_grabed && m_grabed->IsFuel() == false ){
			return;
		}

		//�����i�X���b�g����͎�邾���B
		if( slotNo == Furnace::RESULT ){
			if( result == false ){
				return;
			}

			if(m_othorCtrl && event.IsPressShift() ){
				m_othorCtrl->AddItem( result.GetStack() );
			} else{
				grab.TakeFrom( result, INT_MAX );
			}

			//�΂����Ă����炱���ŏI���B
			if( m_fire.isFire() ){
				return;
			}

			if( source == false ){
				return;
			}

			//���V�s���Q�Ƃ��āA�R�₹����̂������ꍇ�A�R�Ă̂��߂�GameObject�𐶐�����B
			auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

			if( recipeResult == nullptr ){
				return;
			}

			//�����i�X���b�g�Ɏז��ȕ��������Ă���ƔR�₹�Ȃ��B
			if( result ){
				if( recipeResult->GetID() != result.GetID() ){
					return;
				}
				if( result.GetNumber() == result.GetItem()->GetStackLimit() ){
					return;
				}
			}

			//�΂�����B
			m_fire.setFire();

			return;
		}

		//�ʏ�̃N���b�N����B
		InventoryController::OnMouseEvent( event, slotNo );

		//�΂����Ă����炱���ŏI���B
		if( m_fire.isFire() ){
			if( slotNo == Furnace::SOURCE ){
				m_fire.SourceChange();
			}
			return;
		}

		if( source == false || fuel == false ){
			return;
		}

		//���V�s���Q�Ƃ��āA�R�₹����̂������ꍇ�A�R�Ă̂��߂�GameObject�𐶐�����B
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

		if( recipeResult == nullptr ){
			return;
		}

		//�����i�X���b�g�Ɏז��ȕ��������Ă���ƔR�₹�Ȃ��B
		if( result ){
			if( recipeResult->GetID() != result.GetID() ){
				return;
			}
			if( result.GetNumber() == result.GetItem()->GetStackLimit() ){
				return;
			}
		}

		//�΂�����B
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

		//�΂����Ă����炱���ŏI���B
		if( m_fire.isFire() ){
			return;
		}

		if( source == false || fuel == false ){
			return;
		}

		//���V�s���Q�Ƃ��āA�R�₹����̂������ꍇ�A�R�Ă̂��߂�GameObject�𐶐�����B
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

		if( recipeResult == nullptr ){
			return;
		}

		//�����i�X���b�g�Ɏז��ȕ��������Ă���ƔR�₹�Ȃ��B
		auto& result = m_inventory.GetItem( Furnace::RESULT );
		if( result ){
			if( recipeResult->GetID() != result->GetID() ){
				return;
			}
			if( result->GetNumber() == result->GetStackLimit() ){
				return;
			}
		}

		//�΂�����B
		m_fire.setFire();
	}
}