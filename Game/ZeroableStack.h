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

	void SetNumber(int num){
		m_num = num;
		if( m_num == 0 ){
			m_stack = nullptr;
			return;
		}
		if( m_stack == nullptr ){
			m_stack = std::make_unique<ItemStack>( *m_item, 0 );
		}
		m_stack->SetNumber( num );
	}

	int GetID() const{
		if( m_item ){
			return m_item->GetID();
		} else{
			return -1;
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