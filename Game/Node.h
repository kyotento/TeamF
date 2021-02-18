//! @file
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

		//! @brief 末端の子ノードまで再帰的に描画。
		void RecursiveDraw( const CVector2& pos, const CVector2& scale );

	private:
		//! @brief 描画関数。
		virtual void Draw( const CVector2& pos, const CVector2& parentScale ){};
	public:

		//! @brief 最前面に描画する関数。ルートノードしか呼ばれない。
		virtual void DrawForeground(){};

		//! @brief 位置を設定
		void SetPos( const CVector2& pos ){
			m_pos = pos;
		}

		//! @brief 位置を取得。
		CVector2 GetPos()const{
			return m_pos;
		}
		
		//! @brief 左上の頂点位置を取得
		CVector2 GetUpperLeftPos() const;

		//! @brief 元の画像の大きさを1としたときのスケールを設定。
		void SetScale( const CVector2& scale ){
			m_scale = scale;
		}

		//! @brief 元の画像の大きさを1としたときのスケールを取得。
		CVector2 GetScale() const{
			return m_scale;
		}

		//! @brief 基点を設定
		void SetPivot( const CVector2& pivot ){
			m_pivot = pivot;
		}

		//! @brief 基点を取得。
		CVector2 GetPivot()const{
			return m_pivot;
		}

		//! @brief 大きさを取得。xが幅、yが高さ。
		virtual CVector2 GetSize()const = 0;

		//! @brief 座標を自分の座標系に変換する。
		CVector2 GetPosOnThis(CVector2 pos) const{
			pos -= GetUpperLeftPos();
			pos.x /= m_scale.x;
			pos.y /= m_scale.y;
			return pos;
		}

		//! @brief 子ノードを追加。引数はムーブされてemptyになります。
		void AddChilde( std::unique_ptr<Node> childe ){
			m_children.push_back( std::move(childe) );
		}

		//! @brief 子ノードを取得。
		std::vector<std::unique_ptr<Node>>& GetChildren(){
			return m_children;
		}

		//! @brief マウスクリックイベントの受け取りと子ノードへの配給。
		void ReciveClickEvent(const Event::ClickEvent& event , bool isRoot = false);

		//! @brief マウスクリックイベントを処理。
		virtual void OnClick(Event::ClickEvent& event){}

		//! @brief ノードの外側で起きたマウスクリックイベント。ルートノードでしか呼ばれない。
		virtual void OnClickOnOut(const Event::ClickEvent& event ){}

	private:
		CVector2 m_pos;
		CVector2 m_scale{1, 1};
		CVector2 m_pivot;
		std::vector<std::unique_ptr<Node>> m_children;
	};

	//! @brief Node の根。GUIManager の AddRoot と RemoveRoot をやってくれるだけ。
	class RootNode : public Node{
	public:
		RootNode();
		~RootNode();
	};
}

