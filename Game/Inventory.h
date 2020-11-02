//! @file
#pragma once
	
class ItemStack;

//! @brief �C���x���g��B�A�C�e��������q�ɁB
//! @author Takayama
class Inventory
{
public:
	//! @param size �X���b�g�̐�
	Inventory(unsigned size);
	~Inventory();

	//! @brief �A�C�e����ǉ�����B�A�C�e�����E������Ɏg�p����B
	//! @param cursor[in,out] �ǉ�����A�C�e���B�������empty�ɂȂ�܂����A�A�C�e������ꂽ���ꂽ�����c��܂��B
	void AddItem( std::unique_ptr<ItemStack>& item);

	//! @brief �X���b�g�ɑ΂��č��N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	//! @param cursor[in,out] �J�[�\�����ێ����Ă���A�C�e���B���̊֐��͂��̎Q�Ƃ�ҏW���܂��B
	void LClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

	//! @brief �X���b�g�ɑ΂��ĉE�N���b�N�����Ƃ��̑���B
	//! @param slotNo �X���b�g�ԍ�
	//! @param cursor[in,out] �J�[�\�����ێ����Ă���A�C�e���B���̊֐��͂��̎Q�Ƃ�ҏW���܂��B
	void RClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

private:
	//�C���x���g���̒���
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;
	CFont m_font;
};

