//! @file
#pragma once

namespace GUI{
	class Node;
}

namespace GUI::Event{

	//! @brief クリックを伝えるクラス。
	//! @author Takayama
	class MouseEvent{
	public:
		//! @brief マウスのどのボタンかを示す。
		enum Button{
			LEFT,
			RIGHT,
			NONE,
		};


		MouseEvent( Button type, bool isClick ,const CVector2& mousePos ) :
			clickType( type ), isClick(isClick) ,mousePos(mousePos){}
		MouseEvent( const MouseEvent& e, const Node& node );

		//! @brief イベントを消費する。消費されたイベントは子ノードに伝わらない。
		void Consume(){
			isConsume = true;
		}

		//! @retval true イベントは消費済み。
		bool IsConsumed() const{
			return isConsume;
		}

		//! @retval true シフトキーが押されている。
		bool IsPressShift() const{
			return GetKeyInput( VK_SHIFT );
		}

		//! @brief マウスのボタンを返す。
		Button GetButton() const{
			return clickType;
		}

		//! @brief クリックならtrue。乗ってるだけならfalse。
		bool IsClick() const{
			return isClick;
		}

		//! @brief クリック位置を返す。
		const CVector2 GetPos() const{
			return mousePos;
		}

		//! @retval true 指定したノード上でのクリックである。
		bool IsOnNode(const Node& node ) const;

		//! @brief 指定したノードの座標系に変換したイベントを返す。
		MouseEvent CreateEventOnNode( const Node& node ) const{
			return MouseEvent( *this, node );
		}

	private:
		const CVector2 mousePos;
		const Button clickType;
		bool isConsume = false;
		bool isClick = false;
	};
}