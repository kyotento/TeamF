//! @file
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

		//! @brief ���[�̎q�m�[�h�܂ōċA�I�ɕ`��B
		void RecursiveDraw( const CVector2& pos, const CVector2& scale );

	private:
		//! @brief �`��֐��B
		virtual void Draw( const CVector2& pos, const CVector2& parentScale ){};
	public:

		//! @brief �őO�ʂɕ`�悷��֐��B���[�g�m�[�h�����Ă΂�Ȃ��B
		virtual void DrawForeground(){};

		//! @brief �ʒu��ݒ�
		void SetPos( const CVector2& pos ){
			m_pos = pos;
		}

		//! @brief �ʒu���擾�B
		CVector2 GetPos()const{
			return m_pos;
		}
		
		//! @brief ����̒��_�ʒu���擾
		CVector2 GetUpperLeftPos() const;

		//! @brief ���̉摜�̑傫����1�Ƃ����Ƃ��̃X�P�[����ݒ�B
		void SetScale( const CVector2& scale ){
			m_scale = scale;
		}

		//! @brief ���̉摜�̑傫����1�Ƃ����Ƃ��̃X�P�[�����擾�B
		CVector2 GetScale() const{
			return m_scale;
		}

		//! @brief ��_��ݒ�
		void SetPivot( const CVector2& pivot ){
			m_pivot = pivot;
		}

		//! @brief ��_���擾�B
		CVector2 GetPivot()const{
			return m_pivot;
		}

		//! @brief �傫�����擾�Bx�����Ay�������B
		virtual CVector2 GetSize()const = 0;

		//! @brief ���W�������̍��W�n�ɕϊ�����B
		CVector2 GetPosOnThis(CVector2 pos) const{
			pos -= GetUpperLeftPos();
			pos.x /= m_scale.x;
			pos.y /= m_scale.y;
			return pos;
		}

		//! @brief �q�m�[�h��ǉ��B�����̓��[�u�����empty�ɂȂ�܂��B
		void AddChilde( std::unique_ptr<Node> childe ){
			m_children.push_back( std::move(childe) );
		}

		//! @brief �q�m�[�h���擾�B
		std::vector<std::unique_ptr<Node>>& GetChildren(){
			return m_children;
		}

		//! @brief �}�E�X�N���b�N�C�x���g�̎󂯎��Ǝq�m�[�h�ւ̔z���B
		void ReciveClickEvent(const Event::ClickEvent& event , bool isRoot = false);

		//! @brief �}�E�X�N���b�N�C�x���g�������B
		virtual void OnClick(Event::ClickEvent& event){}

		//! @brief �m�[�h�̊O���ŋN�����}�E�X�N���b�N�C�x���g�B���[�g�m�[�h�ł����Ă΂�Ȃ��B
		virtual void OnClickOnOut(const Event::ClickEvent& event ){}

	private:
		CVector2 m_pos;
		CVector2 m_scale{1, 1};
		CVector2 m_pivot;
		std::vector<std::unique_ptr<Node>> m_children;
	};

	//! @brief Node �̍��BGUIManager �� AddRoot �� RemoveRoot ������Ă���邾���B
	class RootNode : public Node{
	public:
		RootNode();
		~RootNode();
	};
}

