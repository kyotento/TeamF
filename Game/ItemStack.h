#pragma once
#include "Item.h"

class ItemStack{
public:

	ItemStack(){}

	//! @brief ����ݒ�B
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief �����擾�B
	int GetNumber(){
		return m_number;
	}

	//! @brief �A�C�e�������擾�B
	Item& GetItem(){
		return m_item;
	}

private:
	Item& m_item;
	int m_number = 0;
};

