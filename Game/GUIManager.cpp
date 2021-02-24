#include "stdafx.h"
#include "GUIManager.h"
#include "Node.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace{
	const int MOUSE_BUTTONS[]{ VK_LBUTTON, VK_RBUTTON };
}

namespace GUI{

	void GUIManager::Update(){
		using namespace GUI::Event;

		//ルートノードが追加されたフレームではクリックイベント出さない
		if (m_isAddRootFrame) {
			m_isAddRootFrame = false;
			return;
		}

		//マウスイベントを発行。
		for( auto& r : m_roots ){
			MouseEvent::Button type = MouseEvent::Button::NONE;
			bool isClick = false;

			//右、左ボタンを調べ、押されていたらtypeに追加。
			for( int button : MOUSE_BUTTONS ){
				if( GetKeyInput(button) ){
					switch( button ){
					case VK_LBUTTON:
						type = MouseEvent::Button::LEFT;
						break;
					case VK_RBUTTON:
						type = MouseEvent::Button::RIGHT;
						break;
					}
					if( GetKeyDown( button ) ){
						isClick = true;
					}
				}
			}

			//マウス座標の作成。
			CVector2 pos = MouseCursor().GetMouseCursorPos();
			auto& grEn = GetGraphicsEngine();
			pos.x *= grEn.GetFrameBuffer_W();
			pos.y *= grEn.GetFrameBuffer_H();

			r->ReciveMouseEvent( MouseEvent( type, isClick ,pos ), true );
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
