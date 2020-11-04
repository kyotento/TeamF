#pragma once

namespace GUI{

	class Node;

	class GUIManager : public IGameObject{
	public:
		static GUIManager& Instance(){
			static GUIManager st_manager;
			return st_manager;
		}

		void AddRoot(Node* root){
			m_roots.insert( root );
		}

		void RemoveRoot( Node* root ){
			m_roots.erase( root );
		}

		void Update() override;

	private:
		GUIManager();
		~GUIManager();

		std::unordered_set<Node*> m_roots;
	};

}
