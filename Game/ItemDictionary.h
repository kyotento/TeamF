//! @file
#pragma once
#include "Item.h"

class BlockInfo;

//! @brief �A�C�e�����Ǘ��̂��߂Ɏg�p�B�V���O���g���B
//! @author Takayama
class ItemDictionary{
public:

	static ItemDictionary& Instance(){
		static ItemDictionary st_storage;
		return st_storage;
	}

	//! @brief �t�H���_���w�肵��json����A�C�e���f�[�^��ǂݍ��ށB
	void LoadItems(std::filesystem::path folder);

	//! @brief BlockInfo����A�C�e���f�[�^���쐬����B
	//! @detail BlockFactory::GetBlockMap() ���g�p���Ĉ����Ɏg���Ă��������B
	void LoadBlocks( const std::unordered_map<EnCube, BlockInfo>& blockMap );

	//! @brief �A�C�e����id�ԍ�����擾�B
	//! @param id EnCube �� EnItem
	Item& GetItem( int id ){
		return m_array[id];
	}

	//! @brief �A�C�e����id�����񂩂�擾�B
	//! @param id EnItem�AEnCube�����̂܂ܕ�����ɂ������́B
	//! @exception std::out_of_range �w�肳�ꂽid�����񂪑��݂��Ȃ��ꍇ�B
	Item& GetItem( const std::string& id ) noexcept(false);

	void SetToolMap();

	EnTool DetermineToolId(std::string itemid);
private:
	ItemDictionary() = default;
	ItemDictionary( const ItemDictionary& is ) = delete;

	std::array<Item, enAllItem_Num> m_array{};

	std::unordered_map<std::string, Item*> m_nameMap;

    const char* m_toolMap[enTool_Num];

	bool m_itemLoaded = false;
	bool m_blockLoaded = false;
};

