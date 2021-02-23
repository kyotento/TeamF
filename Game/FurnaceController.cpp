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

		//�R���X���b�g�ɔR���ȊO�͒u�����Ȃ��B
		if( slotNo == Furnace::FUEL && m_grabed && m_grabed->IsFuel() == false ){
			std::stringstream ss;
			ss << m_grabed->GetID() << "\n";
			OutputDebugStringA( ss.str().c_str() );
			return;
		}

		//�����i�X���b�g����͎�邾���B
		if( slotNo == Furnace::RESULT){
			if( result == nullptr ){
				return;
			}

			//�J�[�\������Ȃ��0�̊����i�Ɠ����A�C�e������������B
			if( m_grabed == nullptr ){
				m_grabed = std::make_unique<ItemStack>( result->GetItem(), 0 );
			}

			if( m_grabed->GetID() == result->GetID() ){
				//�����i�X���b�g����莝���ɃA�C�e���ړ��B
				int total = m_grabed->GetNumber() + result->GetNumber();
				m_grabed->SetNumber( min( total, m_grabed->GetStackLimit() ) );

				//�S�̂���莝���������ƁA�����i�X���b�g�̌��ɂȂ�B
				result->SetNumber( total - m_grabed->GetNumber() );
				if( result->GetNumber() <= 0 ){
					result.reset();
				}
			}
			if (source == nullptr)
			{
				return;
			}
			//���V�s���Q�Ƃ��āA�R�₹����̂������ꍇ�A�R�Ă̂��߂�GameObject�𐶐�����B
			auto recipeResult = RecipeManager::Instance().GetFurnaceResult(source->GetID());

			if (recipeResult == nullptr) {
				return;
			}

			//�����i�X���b�g�Ɏז��ȕ��������Ă���ƔR�₹�Ȃ��B
			if (result) {
				if (recipeResult->GetID() != result->GetID()) {
					return;
				}
				if (result->GetNumber() == result->GetStackLimit()) {
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

		if( source == nullptr || fuel == nullptr ){
			return;
		}

		//���V�s���Q�Ƃ��āA�R�₹����̂������ꍇ�A�R�Ă̂��߂�GameObject�𐶐�����B
		auto recipeResult = RecipeManager::Instance().GetFurnaceResult(source->GetID());

		if( recipeResult == nullptr ){
			return;
		}

		//�����i�X���b�g�Ɏז��ȕ��������Ă���ƔR�₹�Ȃ��B
		if(result){
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