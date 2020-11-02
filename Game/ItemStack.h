//! @file
#pragma once
#include "Item.h"

//! @brief �A�C�e���Ƃ��̐������N���X�B
//! @author Takayama
class ItemStack{
public:

	ItemStack(const Item& item , int number = 1) : m_item(item), m_number(number){}

	//! @brief ����ݒ�B
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief �����擾�B
	int GetNumber() const{
		return m_number;
	}

	//! @brief �A�C�e�������擾�B
	const Item& GetItem() const{
		return m_item;
	}

	//! @brief �A�C�e��id���擾�B
	unsigned GetID() const{
		return m_item.GetID();
	}

	//! @brief �X�^�b�N������擾�B
	int GetStackLimit() const{
		return m_item.GetStackLimit();
	}

private:
	const Item& m_item;
	int m_number = 0;
};

