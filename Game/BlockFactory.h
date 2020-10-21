//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

//! @brief Block �𐶐�����N���X�B
//! @author Takayama
class BlockFactory{
public:

	//! @brief ���f����ǂݍ��ށB���̊֐����n�߂ɌĂԂ��ƁB
	//! @details �C���X�^���V���O�`��Ɏg�p���郂�f����ǂݍ��ށBCreateBlock���O�ɌĂ΂�Ă��Ȃ���΂Ȃ�Ȃ��B
	static void LoadInstancingModels(int instanceMax);

	//! @brief �u���b�N�𐶐�����B
	//! @param blockType �u���b�N�̎�ށB
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );

};

