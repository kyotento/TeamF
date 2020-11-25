//! @file
#pragma once
#include "Item.h"

//! @brief �A�C�e���Ƃ��̐������N���X�B�R�s�[�\�B
//! @author Takayama
class ItemStack{
public:

	ItemStack(Item& item , int number = 1) : m_item(item), m_number(number){}

	bool operator==( const ItemStack& rhs ){
		return m_number == rhs.m_number && &m_item == &( rhs.m_item );
	}

	bool operator!=( const ItemStack& rhs ){
		return !( operator==( rhs ) );
	}

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

	//! @brief �A�C�e�������擾�B
	Item& GetItem(){
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

	//! @brief �`��֐�
	void Draw( const CVector2& pos, const CVector2& parentScale );

private:
	CFont m_font;
	Item& m_item;
	int m_number = 0;
};
