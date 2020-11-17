#include "stdafx.h"
#include "Node.h"
#include "GUIManager.h"
#include "ClickEvent.h"
#include "KeyEvent.h"

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

	void Node::ReciveClickEvent(const Event::ClickEvent& event ){
		//クリックが自分の上で起こった場合にだけ実行。
		if( !event.IsOnNode( *this ) ){
			return;
		}

		//自分の座標系に変更。
		Event::ClickEvent eventOnMe = event.CreateEventOnNode( *this );

		//イベントを処理。
		OnClick( eventOnMe );

		//イベントを消費していたらここで終わり。
		if( eventOnMe.IsConsumed() ){
			return;
		}

		//イベントが消費されなかったら子ノードにまわす。
		for( auto& childe : m_children ){
			childe->ReciveClickEvent( eventOnMe );
		}

	}
}