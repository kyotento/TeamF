#include "stdafx.h"
#include "GUIManager.h"
#include "Node.h"
#include "ClickEvent.h"
#include "KeyEvent.h"

namespace{
	const int MOUSE_BUTTONS[]{ VK_LBUTTON, VK_RBUTTON };
}

namespace GUI{

	void GUIManager::Update(){
		using namespace GUI::Event;

		//クリックイベントを発行。
		for( auto& r : m_roots ){
			for( int button : MOUSE_BUTTONS ){
				if( GetKeyUp( button ) ){
					ClickEvent::ClickType type;

					switch( button ){
					case VK_LBUTTON:
						type = ClickEvent::ClickType::LEFT;
						break;
					case VK_RBUTTON:
						type = ClickEvent::ClickType::RIGHT;
						break;
					}

					//マウス座標の作成。
					CVector2 pos = MouseCursor().GetMouseCursorPos();
					auto& grEn = GetGraphicsEngine();
					pos.x *= grEn.GetFrameBuffer_W();
					pos.y *= grEn.GetFrameBuffer_H();

					r->ReciveClickEvent( ClickEvent( type, pos ) );
				}
			}
		}
	}

	void GUIManager::PostRender(){
		for( auto& r : m_roots ){
			r->RecursiveDraw( {0, 0}, {1, 1} );
		}
		for( auto& r : m_roots ){
			r->DrawForeground();
		}
	}

	GUIManager::GUIManager(){}

	GUIManager::~GUIManager(){}

}
