//! @file
#pragma once

#include "ItemType.h"

//! @brief �A�C�e���̎�ނ�\���N���X�B #GetItem(unsigned) �Ŏ擾����B
//! @details �����܂Ŏ�ނ�\�����̂ŁA���ۂ̃A�C�e���� ItemStack �ň����B
//! @author Takayama
class Item{
public:

	//! @brief �A�C�e��id����A�C�e�����擾�B
	//! @details �u���b�Nid���A�C�e��id�Ƃ��Ĉ������Ƃ��ł���B
	static Item& GetItem(unsigned id);

	//! @brief ���̃A�C�e����id���擾�B
	unsigned GetID() const{
		return m_id;
	}

	//! @brief �X�^�b�N������擾�B
	int GetStackLimit() const{
		return m_limitNumber;
	}

	//! @brief �A�C�e�������擾�B
	const wchar_t* GetItemName() const{
		return m_itemName;
	}

private:
	Item(){}
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber )
		:m_id( enCube ), m_itemName( itemName ), m_limitNumber( limitNumber ){}
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber )
		:m_id( enItem ), m_itemName( itemName ), m_limitNumber( limitNumber ){}

	//! �A�C�e��ID
	unsigned m_id = enCube_None;

	//! �X�^�b�N���
	int m_limitNumber = 64;

	//! �A�C�e����
	const wchar_t* m_itemName = nullptr;
};