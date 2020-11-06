//! @file
#pragma once

namespace GUI{

	class Node;

	//! @brief �o�^����GUI�ɃC�x���g�𔭍s����N���X�B
	//! @author Takayama
	class GUIManager : public IGameObject{
	public:

		//! @brief GUIManager���擾�B
		static GUIManager& Instance(){
			static GUIManager st_manager;
			return st_manager;
		}

		//! @brief ���[�g�m�[�h��ǉ��B
		void AddRoot(Node* root){
			m_roots.insert( root );
		}

		//! @brief ���[�g�m�[�h���폜�B
		void RemoveRoot( Node* root ){
			m_roots.erase( root );
		}

		//! @brief IGameObject �̍X�V�֐��B
		void Update() override;

		//! @brief IGameObject ��2D�`��֐��B
		void PostRender() override;

	private:
		GUIManager();
		~GUIManager();

		//! ��ʂɕ\������GUI�̃��[�g�m�[�h�̃��X�g�B
		std::unordered_set<Node*> m_roots;
	};

}
