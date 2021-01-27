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

	//! @brief �e�u���b�N���f�����������ď��������s�B
	static void FindBlockModel(std::function<void(GameObj::InstancingModel*)> func);

	//! @brief ���f���p�X���擾�B Item �N���X���g�p����B
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief �u���b�N�̂`�`�a�a���擾����B
	static const AABB& GetAABB(EnCube blockType) {
		return BLOCK_AABB_ARRAY[blockType];
	}
	//! @brief �u���b�N�̖��邳���擾����B
	static char GetLight(EnCube blockType) {
		return BLOCK_LIGHT_ARRAY[blockType];
	}

	//! @brief �u���b�N�𐶐�����B
	//! @param blockType �u���b�N�̎�ށB
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );	

private:
	//�u���b�N�`��V�F�[�_�[
	static SkinModelEffectShader m_s_ps;

	//�C���X�^���X��
	static int m_instanceMax;

	//�u���b�N��AABB
	static AABB BLOCK_AABB_ARRAY[enCube_Num];
	//�u���b�N�̖��邳
	static char BLOCK_LIGHT_ARRAY[enCube_Num];
};

