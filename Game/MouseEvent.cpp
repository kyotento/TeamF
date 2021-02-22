#include "stdafx.h"
#include "MouseEvent.h"
#include "Node.h"

namespace GUI::Event{
	MouseEvent::MouseEvent( const MouseEvent & e, const Node & node ) :
		mousePos( node.GetPosOnThis(e.mousePos) ), clickType( e.clickType ),
		isClick(e.isClick ), isConsume( e.isConsume ){}


	bool MouseEvent::IsOnNode( const Node & node ) const{
		const CVector2 pos = mousePos - node.GetUpperLeftPos();

		const CVector3 size = node.GetSize();

		return 0 <= pos.x && pos.x < size.x &&
			0 <= pos.y && pos.y < size.y;
	}

}
