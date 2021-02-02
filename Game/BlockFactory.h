//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

class BlockInfo;

//! @brief Block �𐶐�����N���X�B
//! @author Takayama
class BlockFactory{
public:

	//! @brief json�����g�p���ď������B���̊֐����n�߂ɌĂԂ��ƁB
	static void Init(std::filesystem::path jsonFolder);

	//! @brief ���f���p�X���擾�B
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief �u���b�N�𐶐�����B
	//! @param blockType �u���b�N�̎�ށB
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );

	static const std::unordered_map<EnCube, BlockInfo>& GetBlockMap();
};

