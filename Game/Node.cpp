#include "stdafx.h"
#include "Node.h"
#include "ClickEvent.h"
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

	void Node::ReciveClickEvent(const Event::ClickEvent& event, bool isRoot){
		//�N���b�N�������̏�ŋN�������ꍇ�ɂ������s�B
		if( !event.IsOnNode( *this ) ){
			if( isRoot ){
				OnClickOnOut( event );
			}
			return;
		}

		//�����̍��W�n�ɕύX�B
		Event::ClickEvent eventOnMe = event.CreateEventOnNode( *this );

		//�C�x���g�������B
		OnClick( eventOnMe );

		//�C�x���g������Ă����炱���ŏI���B
		if( eventOnMe.IsConsumed() ){
			return;
		}

		//�C�x���g�������Ȃ�������q�m�[�h�ɂ܂킷�B
		for( auto& childe : m_children ){
			childe->ReciveClickEvent( eventOnMe );
		}

	}
	RootNode::RootNode(){
		GUIManager::Instance().AddRoot( this );
	}
	RootNode::~RootNode(){
		GUIManager::Instance().RemoveRoot( this );
	}
}