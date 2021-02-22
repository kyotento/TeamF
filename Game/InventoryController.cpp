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

		//ボタンを離した。
		if( button == Button::NONE ){
			m_dragSlots.clear();
			m_state = NO_DOWN;
			m_dragItem = nullptr;
			return;
		}

		//取得の操作。
		if( event.IsClick() && m_grabed == nullptr ){
			if( button == Button::LEFT ){
				m_inventory.LClickSlot( slotNo, m_grabed );
			} else if( button == Button::RIGHT ){
				m_inventory.RClickSlot( slotNo, m_grabed );
			}
			m_state = ON_GET;
			return;
		}

		//置く操作。
		if( event.IsClick() && m_grabed ){
			m_state = ON_PUT;
		}
		//置く操作。ドラッグの場合があるためステート変更とは分けている。
		if( m_state == ON_PUT ){

			//対象スロット
			ZeroableStack slot( m_inventory.GetItem( slotNo ) );
			ZeroableStack grab( m_grabed );

			//新しいドラッグ範囲でない
			if( m_dragSlots.count( slotNo ) != 0 ){
				return;
			}

			m_dragSlots.insert( slotNo );

			//1個ずつ置いていく。
			if( button == Button::RIGHT && m_grabed ){
				//違うアイテム。
				if( slot && slot.GetID() != grab.GetID() ){
					return;
				}
				m_inventory.RClickSlot( slotNo, m_grabed );
				return;
			}

			//アイテムを均等に置いていく。
			if( button == Button::LEFT ){
				if( m_dragItem == nullptr ){
					m_dragItem = grab.GetItem();
				}

				int total = grab.GetNumber();

				//ドラッグ中アイテムの総数。
				for( int sNum : m_dragSlots ){
					ZeroableStack drag( m_inventory.GetItem( sNum ) );
					total += drag.GetNumber();
				}

				//総数がスロット数を超えたら何もしない。
				if( m_dragSlots.size() > total ){
					return;
				}

				//分配
				for( int sNum : m_dragSlots ){
					ZeroableStack setSlot( m_inventory.GetItem( sNum ), m_dragItem );
					setSlot.SetNumber( total / m_dragSlots.size() );
				}

				//余りを手元に
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
