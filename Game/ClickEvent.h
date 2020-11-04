#pragma once

namespace GUI{
	class Node;
}

namespace GUI::Event{

	struct ClickEvent{
		enum ClickType{
			LEFT,
			RIGHT
		};


		ClickEvent( ClickType type, const CVector2& mousePos ) : clickType( type ), mousePos(mousePos){}
		ClickEvent( const ClickEvent& e, const Node& node );

		void Consume(){
			isConsume = true;
		}

		bool IsConsumed(){
			return isConsume;
		}

		bool IsPressShift(){
			return GetKeyInput( VK_SHIFT );
		}

		ClickType GetClickType(){
			return clickType;
		}

		bool IsOnNode(const Node& node ) const;

		ClickEvent CreateEventOnNode( const Node& node ) const{
			return ClickEvent( *this, node );
		}

	private:
		const CVector2 mousePos;
		const ClickType clickType;
		bool isConsume = false;
	};
}