//! @file
#pragma once

#include "ItemType.h"

class ItemImage;

//! @brief �A�C�e���̎�ނ�\���N���X�B #GetItem(int) �Ŏ擾����B
//! @details �����܂Ŏ�ނ�\�����̂ŁA���ۂ̃A�C�e���� ItemStack �ň����B
//! @author Takayama
class Item{
	friend class ItemDictionary;
public:
	~Item();

	const Item& operator=( Item&& item );

	//! @brief �A�C�e��id����A�C�e�����擾�B
	//! @details �u���b�Nid���A�C�e��id�Ƃ��Ĉ������Ƃ��ł���B
	static Item& GetItem( int id );

	//! @brief�A�C�e���̕�����id����A�C�e�����擾�B
	//! @param strId EnItem�AEnCube�����̂܂ܕ�����ɂ������́B
	//! @exception std::out_of_range �w�肳�ꂽid�����񂪑��݂��Ȃ��ꍇ�B
	static Item& GetItem( const std::string& strId ) noexcept(false);

	//! @brief ���̃A�C�e����id���擾�B
	int GetID() const{
		return m_id;
	}

	//! @brief ���̃A�C�e���̑������擾�B
	int GetToolID() const {
		return m_toolId;
	}

	//! @brief ���̃A�C�e���̃c�[�����x��(��:1�`�_�C��:5)���擾
	int GetToolLevel() const{
		return m_toolLevel;
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

	/// <summary>
	/// �u���b�N���c�[�������擾����B
	/// </summary>
	/// <returns>true�Ȃ�u���b�N</returns>
	bool GetIsBlock() {
		return m_id < enCube_Num;	
	}

	//! @brief �A�C�e����3D���f���̃p�X���擾�B
	std::filesystem::path GetModelPath() const{
		return m_modelPath;
	}

private:
	Item();
	Item( Item&& item );
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber, const std::filesystem::path& modelPath );
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber, const std::filesystem::path& spritePath, const std::filesystem::path& modelPath );

	//! �A�C�e��ID
	int m_id = enCube_None;

	//! �X�^�b�N���
	int m_limitNumber = 64;

	//! �A�C�e����
	const wchar_t* m_itemName = nullptr;

	//�A�C�e���摜�B
	std::unique_ptr<ItemImage> m_image;

	static constexpr int INSTANCE_MODEL_MAX = 500;

	//3D���f���̃p�X�B
	std::filesystem::path m_modelPath;

	//�A�C�e���̑����B
	int m_toolId = enTool_None;

	//�c�[�����x���B��:1����_�C��:5�܂�
	int m_toolLevel = 1;
};