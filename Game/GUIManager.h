//! @file
#pragma once

namespace GUI{

	class Node;

	//! @brief 登録したGUIにイベントを発行するクラス。
	//! @author Takayama
	class GUIManager : public IGameObject{
	public:

		//! @brief GUIManagerを取得。
		static GUIManager& Instance(){
			static GUIManager st_manager;
			return st_manager;
		}

		//! @brief ルートノードを追加。
		void AddRoot(Node* root){
			m_roots.insert( root );
		}

		//! @brief ルートノードを削除。
		void RemoveRoot( Node* root ){
			m_roots.erase( root );
		}

		//! @brief IGameObject の更新関数。
		void Update() override;

		//! @brief IGameObject の2D描画関数。
		void PostRender() override;

	private:
		GUIManager();
		~GUIManager();

		//! 画面に表示するGUIのルートノードのリスト。
		std::unordered_set<Node*> m_roots;
	};

}
