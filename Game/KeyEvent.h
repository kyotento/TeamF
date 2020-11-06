//! @file
#pragma once

namespace GUI::Event{

	//! @brief �L�[�������ꂽ���Ƃ�`����N���X�B
	//! @author Takayama
	class KeyEvent{
	public:
		KeyEvent(int keyCode)
			: keyCode(keyCode){}

		//! @brief �C�x���g�������B����ꂽ�C�x���g�͎q�m�[�h�ɓ`���Ȃ��B
		void Consume(){
			isConsume = true;
		}

		//! @retval true �C�x���g�͏���ς݁B
		bool IsConsumed() const{
			return isConsume;
		}

		//! @brief ���z�L�[�R�[�h���擾�B
		int GetKey() const{
			return keyCode;
		}

		//! @retval true �V�t�g�L�[��������Ă���B
		bool IsPressShift() const{
			return GetKeyInput( VK_SHIFT );
		}
		
	private:
		const int keyCode;
		bool isConsume = false;
	};
}