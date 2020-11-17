//! @file
#pragma once

class ItemStack;

//! @brief �C���x���g��B�A�C�e��������q�ɁB
//! @author Takayama
class Inventory{
public:
	//! @param size �X���b�g�̐�
	Inventory( unsigned size );
	~Inventory();

	//! @brief �A�C�e�����X���b�g�ɃZ�b�g�B�X���b�g���㏑�����܂��B
	void SetItem( unsigned slotNo, std::unique_ptr<ItemStack> item );

	//! @brief �J�[�\���̎蒆�ɃA�C�e�����Z�b�g�B������͂�ł��镨���㏑�����܂��B
	void SetGrabItem( std::unique_ptr<ItemStack> item );

	//! @brief �A�C�e�����擾�B
	ItemStack* GetItem( unsigned slotNo ){
		return m_slotArray[slotNo].get();
	}

	//! @brief �J�[�\�����͂�ł���A�C�e�����擾�B
	ItemStack* GetGrabItem(){
		return m_grabedItem.get();
	}

	//! @brief �A�C�e����ǉ�����B�A�C�e�����E������Ɏg�p����B
	//! @param cursor[in,out] �ǉ�����A�C�e���B�������empty�ɂȂ�܂����A�A�C�e������ꂽ���ꂽ�����c��܂��B
	void AddItem( std::unique_ptr<ItemStack>& item );

	//! @brief �X���b�g�ɑ΂��č��N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	void LClickSlot( unsigned slotNo );

	//! @brief �X���b�g�ɑ΂��ĉE�N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	void RClickSlot( unsigned slotNo );

private:
	//! �C���x���g���̒����B
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;

	//! �J�[�\�����͂�ł���A�C�e���B
	std::unique_ptr<ItemStack> m_grabedItem;
	CFont m_font;
};

