#pragma once

namespace GUI::Event{

	struct KeyEvent{

		KeyEvent(int keyCode)
			: keyCode(keyCode){}

		void Consume(){
			isConsume = true;
		}

		bool IsConsumed() const{
			return isConsume;
		}

		int GetKey() const{
			return keyCode;
		}

		bool IsPressShift() const{
			return GetKeyInput( VK_SHIFT );
		}
		
	private:
		const int keyCode;
		bool isConsume = false;
	};
}