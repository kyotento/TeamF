#pragma once
#include "ItemStack.h"

class ZeroableStack{
public:
	ZeroableStack( std::unique_ptr<ItemStack>& stack, Item* item )
		:m_stack( stack ), m_item(item){
		if( stack ){
			m_item = &stack->GetItem();
			m_num = stack->GetNumber();
		}
	}

	ZeroableStack( std::unique_ptr<ItemStack>& stack ): ZeroableStack(stack, nullptr){}

	int GetNumber() const{
		return m_num;
	}

	void SetNumber( int num );

	void AddNumber( int num ){
		SetNumber( m_num + num );
	}

	//! @brief �������玩���Ɉړ��ł���A�C�e���̐��B
	int GetCanTakeFrom( ZeroableStack& stack );

	//! @brief �������玩���ɃA�C�e�����ړ�������B��ނ��Ⴄ��NG�B�X�^�b�N����܂ł����ړ����Ȃ��B
	//! @return ���ۂɈړ��������B
	int TakeFrom( ZeroableStack& stack, int count );

	int GetID() const{
		if( m_item ){
			return m_item->GetID();
		} else{
			return enCube_None;
		}
	}

	Item* GetItem(){
		return m_item;
	}

	void SetItemToNull(Item* item){
		if( m_item == nullptr ){
			m_item = item;
		}
	}

	void swap( ZeroableStack& stack );

	std::unique_ptr<ItemStack>& GetStack(){
		return m_stack;
	}

	operator bool(){
		return m_stack != nullptr;
	}

private:
	Item* m_item = nullptr;
	int m_num = 0;
	std::unique_ptr<ItemStack>& m_stack;
};