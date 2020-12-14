//! @file
#pragma once

namespace GUI{
	class Node;
}

namespace GUI::Event{

	//! @brief �N���b�N��`����N���X�B
	//! @author Takayama
	class ClickEvent{
	public:
		//! @brief �}�E�X�̂ǂ̃{�^�����������B
		enum ClickType{
			LEFT,
			RIGHT
		};


		ClickEvent( ClickType type, const CVector2& mousePos ) : clickType( type ), mousePos(mousePos){}
		ClickEvent( const ClickEvent& e, const Node& node );

		//! @brief �C�x���g�������B����ꂽ�C�x���g�͎q�m�[�h�ɓ`���Ȃ��B
		void Consume(){
			isConsume = true;
		}

		//! @retval true �C�x���g�͏���ς݁B
		bool IsConsumed() const{
			return isConsume;
		}

		//! @retval true �V�t�g�L�[��������Ă���B
		bool IsPressShift(){
			return GetKeyInput( VK_SHIFT );
		}

		//! @brief �}�E�X�̃{�^����Ԃ��B
		ClickType GetClickType() const{
			return clickType;
		}

		//! @brief �N���b�N�ʒu��Ԃ��B
		const CVector2 GetPos() const{
			return mousePos;
		}

		//! @retval true �w�肵���m�[�h��ł̃N���b�N�ł���B
		bool IsOnNode(const Node& node ) const;

		//! @brief �w�肵���m�[�h�̍��W�n�ɕϊ������C�x���g��Ԃ��B
		ClickEvent CreateEventOnNode( const Node& node ) const{
			return ClickEvent( *this, node );
		}

	private:
		const CVector2 mousePos;
		const ClickType clickType;
		bool isConsume = false;
	};
}