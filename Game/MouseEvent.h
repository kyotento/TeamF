//! @file
#pragma once

namespace GUI{
	class Node;
}

namespace GUI::Event{

	//! @brief �N���b�N��`����N���X�B
	//! @author Takayama
	class MouseEvent{
	public:
		//! @brief �}�E�X�̂ǂ̃{�^�����������B
		enum Button{
			LEFT,
			RIGHT,
			NONE,
		};


		MouseEvent( Button type, bool isClick ,const CVector2& mousePos ) :
			clickType( type ), isClick(isClick) ,mousePos(mousePos){}
		MouseEvent( const MouseEvent& e, const Node& node );

		//! @brief �C�x���g�������B����ꂽ�C�x���g�͎q�m�[�h�ɓ`���Ȃ��B
		void Consume(){
			isConsume = true;
		}

		//! @retval true �C�x���g�͏���ς݁B
		bool IsConsumed() const{
			return isConsume;
		}

		//! @retval true �V�t�g�L�[��������Ă���B
		bool IsPressShift() const{
			return GetKeyInput( VK_SHIFT );
		}

		//! @brief �}�E�X�̃{�^����Ԃ��B
		Button GetButton() const{
			return clickType;
		}

		//! @brief �N���b�N�Ȃ�true�B����Ă邾���Ȃ�false�B
		bool IsClick() const{
			return isClick;
		}

		//! @brief �N���b�N�ʒu��Ԃ��B
		const CVector2 GetPos() const{
			return mousePos;
		}

		//! @retval true �w�肵���m�[�h��ł̃N���b�N�ł���B
		bool IsOnNode(const Node& node ) const;

		//! @brief �w�肵���m�[�h�̍��W�n�ɕϊ������C�x���g��Ԃ��B
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