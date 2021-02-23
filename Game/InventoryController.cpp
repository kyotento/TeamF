#include "stdafx.h"
#include "InventoryController.h"
#include "Inventory.h"
#include "MouseEvent.h"
#include "ItemStack.h"
#include "ZeroableStack.h"

namespace GUI::Controller{
	InventoryController::InventoryController( Inventory & inventory, std::unique_ptr<ItemStack>& grabed ) :
		m_inventory( inventory ), m_grabed( grabed ){}

	void InventoryController::OnMouseEvent( Event::MouseEvent & event, unsigned slotNo ){
		using Button = Event::MouseEvent::Button;
		Button button = event.GetButton();

		//�{�^���𗣂����B
		if( button == Button::NONE ){
			m_dragSlots.clear();
			m_state = NO_DOWN;
			m_dragItem = nullptr;
			return;
		}

		//�擾�̑���B
		if( event.IsClick() && m_grabed == nullptr ){
			if( button == Button::LEFT ){
				m_inventory.LClickSlot( slotNo, m_grabed );
			} else if( button == Button::RIGHT ){
				m_inventory.RClickSlot( slotNo, m_grabed );
			}
			m_state = ON_GET;
			return;
		}

		//�u������B
		if( event.IsClick() && m_grabed ){
			m_state = ON_PUT;
		}
		//�u������B�h���b�O�̏ꍇ�����邽�߃X�e�[�g�ύX�Ƃ͕����Ă���B
		if( m_state == ON_PUT ){

			//�ΏۃX���b�g
			ZeroableStack slot( m_inventory.GetItem( slotNo ) );
			ZeroableStack grab( m_grabed );

			//�V�����h���b�O�͈͂łȂ�
			if( m_dragSlots.count( slotNo ) != 0 ){
				return;
			}

			m_dragSlots.insert( slotNo );

			//1���u���Ă����B
			if( button == Button::RIGHT && m_grabed ){
				//�Ⴄ�A�C�e���B
				if( slot && slot.GetID() != grab.GetID() ){
					return;
				}
				m_inventory.RClickSlot( slotNo, m_grabed );
				return;
			}

			//�A�C�e�����ϓ��ɒu���Ă����B
			if( button == Button::LEFT ){
				if( m_dragItem == nullptr ){
					m_dragItem = grab.GetItem();
				}

				int total = grab.GetNumber();

				//�h���b�O���A�C�e���̑����B
				for( int sNum : m_dragSlots ){
					ZeroableStack drag( m_inventory.GetItem( sNum ) );
					total += drag.GetNumber();
				}

				//�������X���b�g���𒴂����牽�����Ȃ��B
				if( m_dragSlots.size() > total ){
					return;
				}

				//���z
				for( int sNum : m_dragSlots ){
					ZeroableStack setSlot( m_inventory.GetItem( sNum ), m_dragItem );
					setSlot.SetNumber( total / m_dragSlots.size() );
				}

				//�]����茳��
				grab.SetItemToNull( m_dragItem );
				grab.SetNumber( total % m_dragSlots.size());
				return;
			}
			return;
		}
	}

	bool InventoryController::ClickGet( Event::MouseEvent & event, unsigned slotNo ){
		using Button = Event::MouseEvent::Button;
		Button button = event.GetButton();

		if( event.IsClick() && m_grabed == nullptr ){
			if( button == Button::LEFT ){
				m_inventory.LClickSlot( slotNo, m_grabed );
			} else if( button == Button::RIGHT ){
				m_inventory.RClickSlot( slotNo, m_grabed );
			}
			m_state = ON_GET;
			return true;
		}
		return false;
	}

}
