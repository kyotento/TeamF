//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

//! @brief Block �𐶐�����N���X�B
//! @author Takayama
class BlockFactory{
public:

	//! @brief json�����g�p���ď������B���̊֐����n�߂ɌĂԂ��ƁB
	static void Init(std::filesystem::path jsonFolder);

	//! @brief ���f���p�X���擾�B Item �N���X���g�p����B
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief �u���b�N�𐶐�����B
	//! @param blockType �u���b�N�̎�ށB
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );
};

