#include "stdafx.h"
#include "Node.h"
#include "GUIManager.h"
#include "ClickEvent.h"
#include "KeyEvent.h"

namespace GUI{

	Node::Node(){}

	Node::~Node(){
		GUIManager::Instance().RemoveRoot( this );
	}

	void Node::ReciveKeyDownEvent( Event::KeyEvent && event ){
		OnKeyDown( event );

		//�C�x���g������Ă����炱���ŏI���B
		if( event.IsConsumed() ){
			return;
		}

		//�C�x���g�������Ȃ�������q�m�[�h�ɂ܂킷�B
		for( auto& childe : m_children ){
			childe->ReciveKeyDownEvent( std::move(event) );
		}
	}

	void Node::ReciveClickEvent( Event::ClickEvent && event ){
		//�N���b�N�������̏�ŋN�������ꍇ�ɂ������s�B
		if( !event.IsOnNode( *this ) ){
			return;
		}

		//�C�x���g�������B
		OnClick( event );

		//�C�x���g������Ă����炱���ŏI���B
		if( event.IsConsumed() ){
			return;
		}

		//�C�x���g�������Ȃ�������q�m�[�h�ɂ܂킷�B
		for( auto& childe : m_children ){
			childe->ReciveClickEvent( event.CreateEventOnNode( *this ) );
		}

	}