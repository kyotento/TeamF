#include "stdafx.h"
#include "ClickEvent.h"
#include "Node.h"

namespace GUI::Event{
	ClickEvent::ClickEvent( const ClickEvent & e, const Node & node ) :
		mousePos( node.GetPosOnThis(e.mousePos) ), clickType( e.clickType ), isConsume( e.isConsume ){}


	bool ClickEvent::IsOnNode( const Node & node ) const{
		const CVector2 pos = mousePos - node.GetUpperLeftPos();

		const CVector3 size = node.GetSize();

		return 0 <= pos.x && pos.x <= size.x &&
			0 <= pos.y && pos.y <= size.y;
	}

}
