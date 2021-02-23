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

		//���[�g�m�[�h���ǉ����ꂽ�t���[���ł̓N���b�N�C�x���g�o���Ȃ�
		if (m_isAddRootFrame) {
			m_isAddRootFrame = false;
			return;
		}

		//�}�E�X�C�x���g�𔭍s�B
		for( auto& r : m_roots ){
			MouseEvent::Button type = MouseEvent::Button::NONE;
			bool isClick = false;

			//�E�A���{�^���𒲂ׁA������Ă�����type�ɒǉ��B
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

			//�}�E�X���W�̍쐬�B
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
