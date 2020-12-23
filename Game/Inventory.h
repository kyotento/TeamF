//! @file
#pragma once

class NullableItemStack;
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

	//! @brief �A�C�e�����擾�B
	std::unique_ptr<ItemStack>& GetItem( unsigned slotNo ){
		return m_slotArray[slotNo];
	}

	//! @brief �A�C�e�����擾�B�w�肵���X���b�g��null�Ȃ�Anull�I�u�W�F�N�g��Ԃ��B
	NullableItemStack& GetNullableItem( unsigned slotNo );

	//! @brief �w�肵���������A�X���b�g����A�C�e�������B
	//! @details ���ʂ�Get�͎Q�Ƃ̎擾�ŁA�������̓C���x���g������߂�l�փA�C�e�����ړ�������B
	std::unique_ptr<ItemStack> TakeItem( unsigned slotNo, int num );

	//! @brief �w�肵���������X���b�g����A�C�e���������B
	void DeleteItem( unsigned slotNo, int num );

	auto begin(){
		return m_slotArray.begin();
	}

	auto end(){
		return m_slotArray.end();
	}

	//! @brief �A�C�e����ǉ�����B�A�C�e�����E������Ɏg�p����B
	//! @param cursor[in,out] �ǉ�����A�C�e���B�������empty�ɂȂ�܂����A�A�C�e������ꂽ���ꂽ�����c��܂��B
	void AddItem( std::unique_ptr<ItemStack>& item );

	//! @brief �X���b�g�ɑ΂��č��N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	//! @param cursor[in, out] �J�[�\���������Ă���A�C�e���B
	void LClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

	//! @brief �X���b�g�ɑ΂��ĉE�N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	//! @param cursor[in, out] �J�[�\���������Ă���A�C�e���B
	void RClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

private:
	//! �C���x���g���̒����B
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;

	CFont m_font;
};

