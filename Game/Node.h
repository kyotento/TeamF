#pragma once

namespace GUI{

	namespace Event{
		class KeyEvent;
		class ClickEvent;
	}

	//! @brief ���ׂĂ�GUI�I�u�W�F�N�g�̊��N���X�B�����̎q�m�[�h�����B
	//! @author Takayama
	class Node{
	public:
		Node();
		virtual ~Node();

		//! @brief �ʒu��ݒ�
		void SetPos( const CVector2& pos ){
			m_pos = pos;
		}

		//! @brief �ʒu���擾�B
		CVector2 GetPos()const{
			return m_pos;
		}

		//! @brief �傫����ݒ�Bx�����Ay�������B
		void SetSize( const CVector2& size ){
			m_size = size;
		}

		//! @brief �傫�����擾�Bx�����Ay�������B
		CVector2 GetSize()const{
			return m_size;
		}

		//! @brief �q�m�[�h��ǉ��B�����̓��[�u�����empty�ɂȂ�܂��B
		void AddChilde( std::unique_ptr<Node> childe ){
			m_children.push_back( std::move(childe) );
		}

		//! @brief �q�m�[�h���擾�B
		std::vector<std::unique_ptr<Node>>& GetChildren(){
			return m_children;
		}

		//! @brief �L�[���̓C�x���g�̎󂯎��Ǝq�m�[�h�ւ̔z���B
		void ReciveKeyDownEvent( Event::KeyEvent&& event );

		//! @brief �L�[���̓C�x���g�������B
		virtual void OnKeyDown( Event::KeyEvent& keyCode ){}

		//! @brief �}�E�X�N���b�N�C�x���g�̎󂯎��Ǝq�m�[�h�ւ̔z���B
		void ReciveClickEvent( Event::ClickEvent&& event );

		//! @brief �}�E�X�N���b�N�C�x���g�������B
		virtual void OnClick(Event::ClickEvent& event){}

	private:
		CVector2 m_pos;
		CVector2 m_size;

		std::vector<std::unique_ptr<Node>> m_children;
	};

}

