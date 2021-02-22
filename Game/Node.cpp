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

		//マウスが自分の外側なら終了。
		if( isOnMe == false){
			//ルートノードだけ外側クリックを拾う。
			if( event.IsClick() && isRoot ){
				OnClickOnOut( event );
			}
			return;
		}

		//自分の座標系に変更。
		Event::MouseEvent eventOnMe = event.CreateEventOnNode( *this );

		if( isOnMe ){
			//イベント発行
			OnMouseEvent( eventOnMe );

			//イベントを消費していたらここで終わり。
			if( eventOnMe.IsConsumed() ){
				return;
			}
		}

		//イベントが消費されなかったら子ノードにまわす。
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