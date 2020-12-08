//! @file
#pragma once

#include "ItemType.h"

class ItemImage;

//! @brief �A�C�e���̎�ނ�\���N���X�B #GetItem(unsigned) �Ŏ擾����B
//! @details �����܂Ŏ�ނ�\�����̂ŁA���ۂ̃A�C�e���� ItemStack �ň����B
//! @author Takayama
class Item{
	friend class ItemDictionary;
public:
	~Item();

	const Item& operator=( Item&& item );

	//! @brief �A�C�e��id����A�C�e�����擾�B
	//! @details �u���b�Nid���A�C�e��id�Ƃ��Ĉ������Ƃ��ł���B
	static Item& GetItem( unsigned id );

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

	//! @brief GUI�p�̃A�C�e���̕`��B
	void Draw( const CVector2& pos, const CVector2& scale );

private:
	Item();
	Item( Item&& item );
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber, const std::filesystem::path& modelPath );
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber, const std::filesystem::path& spritePath );

	//! �A�C�e��ID
	unsigned m_id = enCube_None;

	//! �X�^�b�N���
	int m_limitNumber = 64;

	//! �A�C�e����
	const wchar_t* m_itemName = nullptr;

	//�A�C�e���摜�A�܂��̓��f���B
	std::unique_ptr<ItemImage> m_image;
};