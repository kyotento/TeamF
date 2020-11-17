//! @file
#pragma once

namespace GUI{
	class Node;
}

namespace GUI::Event{

	//! @brief クリックを伝えるクラス。
	//! @author Takayama
	class ClickEvent{
	public:
		//! @brief マウスのどのボタンかを示す。
		enum ClickType{
			LEFT,
			RIGHT
		};


		ClickEvent( ClickType type, const CVector2& mousePos ) : clickType( type ), mousePos(mousePos){}
		ClickEvent( const ClickEvent& e, const Node& node );

		//! @brief イベントを消費する。消費されたイベントは子ノードに伝わらない。
		void Consume(){
			isConsume = true;
		}

		//! @retval true イベントは消費済み。
		bool IsConsumed() const{
			return isConsume;
		}

		//! @retval true シフトキーが押されている。
		bool IsPressShift(){
			return GetKeyInput( VK_SHIFT );
		}

		//! @brief マウスのボタンを返す。
		ClickType GetClickType() const{
			return clickType;
		}

		//! @brief クリック位置を返す。
		const CVector2 GetPos() const{
			return mousePos;
		}

		//! @retval true 指定したノード上でのクリックである。
		bool IsOnNode(const Node& node ) const;

		//! @brief 指定したノードの座標系に変換したイベントを返す。
		ClickEvent CreateEventOnNode( const Node& node ) const{
			return ClickEvent( *this, node );
		}

	private:
		const CVector2 mousePos;
		const ClickType clickType;
		bool isConsume = false;
	};
}