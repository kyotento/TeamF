#pragma once

namespace GUI{

	namespace Event{
		class KeyEvent;
		class ClickEvent;
	}

	//! @brief すべてのGUIオブジェクトの基底クラス。複数の子ノードを持つ。
	//! @author Takayama
	class Node{
	public:
		Node();
		virtual ~Node();

		//! @brief 位置を設定
		void SetPos( const CVector2& pos ){
			m_pos = pos;
		}

		//! @brief 位置を取得。
		CVector2 GetPos()const{
			return m_pos;
		}

		//! @brief 大きさを設定。xが幅、yが高さ。
		void SetSize( const CVector2& size ){
			m_size = size;
		}

		//! @brief 大きさを取得。xが幅、yが高さ。
		CVector2 GetSize()const{
			return m_size;
		}

		//! @brief 子ノードを追加。引数はムーブされてemptyになります。
		void AddChilde( std::unique_ptr<Node> childe ){
			m_children.push_back( std::move(childe) );
		}

		//! @brief 子ノードを取得。
		std::vector<std::unique_ptr<Node>>& GetChildren(){
			return m_children;
		}

		//! @brief キー入力イベントの受け取りと子ノードへの配給。
		void ReciveKeyDownEvent( Event::KeyEvent&& event );

		//! @brief キー入力イベントを処理。
		virtual void OnKeyDown( Event::KeyEvent& keyCode ){}

		//! @brief マウスクリックイベントの受け取りと子ノードへの配給。
		void ReciveClickEvent( Event::ClickEvent&& event );

		//! @brief マウスクリックイベントを処理。
		virtual void OnClick(Event::ClickEvent& event){}

	private:
		CVector2 m_pos;
		CVector2 m_size;

		std::vector<std::unique_ptr<Node>> m_children;
	};

}

