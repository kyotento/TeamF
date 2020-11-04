#include "stdafx.h"
#include "GUIManager.h"
#include "Node.h"
#include "ClickEvent.h"
#include "KeyEvent.h"

namespace{
	const int NEED_OBSERVE_KEYS[]{'E'};
	const int MOUSE_BUTTONS[]{ VK_LBUTTON, VK_RBUTTON };
}

namespace GUI{

	void GUIManager::Update(){

		for( int key : NEED_OBSERVE_KEYS ){

			if( GetKeyDown( key ) ){

				for( auto& r : m_roots ){
					
					//r->ReciveKeyDownEvent( keyEvent );
				}
			}
		}

		for( int button : MOUSE_BUTTONS ){

			if( GetKeyDown( button ) ){

				for( auto& r : m_roots ){
					using namespace GUI::Event;

					ClickEvent::ClickType type;

					switch( button ){
					case VK_LBUTTON:
						type = ClickEvent::ClickType::LEFT;
						break;
					case VK_RBUTTON:
						type = ClickEvent::ClickType::RIGHT;
						break;
					}

					CVector2 pos = MouseCursor().GetMouseCursorClientPos();

					r->ReciveClickEvent( ClickEvent( type, pos ) );
				}

			}

		}
	}

	GUIManager::GUIManager(){}

	GUIManager::~GUIManager(){}

}
