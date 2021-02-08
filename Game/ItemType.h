#pragma once

#include "BlockType.h"

enum EnItem{
	enItem_Rod = enCube_Num,
	enItem_Coal,
	enItem_Charcoal,
	enItem_Iron_Ingot,
	enItem_Gold_Ingot,
	enItem_Diamond,

	//�؂̃c�[����
	enItem_Wood_Sword,
	enItem_Wood_Pickaxe,
	enItem_Wood_Shovel,
	enItem_Wood_Axe,
	enItem_Wood_Hoe,

	//�΂̃c�[����
	enItem_Stone_Sword,
	enItem_Stone_Pickaxe,
	enItem_Stone_Shovel,
	enItem_Stone_Axe,
	enItem_Stone_Hoe,

	//�S�̃c�[����
	enItem_Iron_Sword,
	enItem_Iron_Pickaxe,
	enItem_Iron_Shovel,
	enItem_Iron_Axe,
	enItem_Iron_Hoe,

	//���̃c�[����
	enItem_Gold_Sword,
	enItem_Gold_Pickaxe,
	enItem_Gold_Shovel,
	enItem_Gold_Axe,
	enItem_Gold_Hoe,

	//�_�C���̃c�[����
	enItem_Diamond_Sword,
	enItem_Diamond_Pickaxe,
	enItem_Diamond_Shovel,
	enItem_Diamond_Axe,
	enItem_Diamond_Hoe,

	//�u���b�N���܂ނ��ׂẴA�C�e���̐�
	enAllItem_Num,
};

//�A�C�e���̑��������߂�A
enum EnTool
{
	enTool_None = -1,		//�����Ȃ��B
	enTool_Sword,		//���B
	enTool_Pickaxe,		//�s�b�P���B
	enTool_Shovel,		//�V���x���B
	enTool_Axe,			//���B
	enTool_Hoe,			//�L�B
	enTool_Helmet,		//�w�����b�g�B
	enTool_Plate,		//�v���[�g�B
	enTool_Leggings,	//���M���X(�Y�{��)�B
	enTool_Boots,		//�u�[�c�B

	enTool_Num,			//�����̐��B
};