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

		//イベントを消費していたらここで終わり。
		if( event.IsConsumed() ){
			return;
		}

		//イベントが消費されなかったら子ノードにまわす。
		for( auto& childe : m_children ){
			childe->ReciveKeyDownEvent( std::move(event) );
		}
	}

	void Node::ReciveClickEvent( Event::ClickEvent && event ){
		//クリックが自分の上で起こった場合にだけ実行。
		if( !event.IsOnNode( *this ) ){
			return;
		}

		//イベントを処理。
		OnClick( event );

		//イベントを消費していたらここで終わり。
		if( event.IsConsumed() ){
			return;
		}

		//イベントが消費されなかったら子ノードにまわす。
		for( auto& childe : m_children ){
			childe->ReciveClickEvent( event.CreateEventOnNode( *this ) );
		}

	}