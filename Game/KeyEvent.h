//! @file
#pragma once

namespace GUI::Event{

	//! @brief キーが押されたことを伝えるクラス。
	//! @author Takayama
	class KeyEvent{
	public:
		KeyEvent(int keyCode)
			: keyCode(keyCode){}

		//! @brief イベントを消費する。消費されたイベントは子ノードに伝わらない。
		void Consume(){
			isConsume = true;
		}

		//! @retval true イベントは消費済み。
		bool IsConsumed() const{
			return isConsume;
		}

		//! @brief 仮想キーコードを取得。
		int GetKey() const{
			return keyCode;
		}

		//! @retval true シフトキーが押されている。
		bool IsPressShift() const{
			return GetKeyInput( VK_SHIFT );
		}
		
	private:
		const int keyCode;
		bool isConsume = false;
	};
}