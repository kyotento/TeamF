#include "stdafx.h"
#include "Node.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "GUIManager.h"

namespace GUI{

	Node::Node(){}

	Node::~Node(){}

	void Node::RecursiveDraw( const CVector2& parentsZero, const CVector2& parentScale ){

		Draw( parentsZero + (m_pos * parentScale), parentScale);

		const CVector2 inMeScale{ parentScale.x * m_scale.x, parentScale.y * m_scale.y };
		for( auto& childe : m_children ){
			childe->RecursiveDraw( parentsZero + (GetUpperLeftPos() * parentScale ), inMeScale);
		}
	}

	CVector2 Node::GetUpperLeftPos() const{
		CVector2 pivotSize = m_pivot;
		const CVector2 size = GetSize();
		pivotSize.x *= size.x;
		pivotSize.y *= size.y;
		return m_pos - pivotSize;
	}

	void Node::ReciveMouseEvent(const Event::MouseEvent& event, bool isRoot){
		const bool isOnMe = event.IsOnNode( *this );

		//�}�E�X�������̊O���Ȃ�I���B
		if( isOnMe == false){
			//���[�g�m�[�h�����O���N���b�N���E���B
			if( event.IsClick() && isRoot ){
				OnClickOnOut( event );
			}
			return;
		}

		//�����̍��W�n�ɕύX�B
		Event::MouseEvent eventOnMe = event.CreateEventOnNode( *this );

		if( isOnMe ){
			//�C�x���g���s
			OnMouseEvent( eventOnMe );

			//�C�x���g������Ă����炱���ŏI���B
			if( eventOnMe.IsConsumed() ){
				return;
			}
		}

		//�C�x���g�������Ȃ�������q�m�[�h�ɂ܂킷�B
		for( auto& childe : m_children ){
			childe->ReciveMouseEvent( eventOnMe );
		}

	}
	RootNode::RootNode(){
		GUIManager::Instance().AddRoot( this );
	}
	RootNode::~RootNode(){
		GUIManager::Instance().RemoveRoot( this );
	}
}