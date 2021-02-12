#pragma once

#include "BlockType.h"

enum EnItem{
	enItem_Rod = enCube_Num,
	enItem_Coal,
	enItem_Charcoal,
	enItem_Iron_Ingot,
	enItem_Gold_Ingot,
	enItem_Diamond,
	enItem_Leather,//��

	//�H��
	enItem_Raw_Meat,
	enItem_Steak,

	//�؂̃c�[����
	enItem_Wood_Sword,
	enItem_Wood_Pickaxe,
	enItem_Wood_Shovel,
	enItem_Wood_Axe,
	enItem_Wood_Hoe,

	//��̃c�[���ށB
	enItem_Leather_Helmet,
	enItem_Leather_ChestPlate,
	enItem_Leather_ChestPlate2,
	enItem_Leather_Leggings,
	enItem_Leather_Boots,


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

	//�h��B
	enItem_Iron_Helmet,
	enItem_Iron_ChestPlate,
	enItem_Iron_ChestPlate2,
	enItem_Iron_Leggings,
	enItem_Iron_Boots,


	//���̃c�[����
	enItem_Gold_Sword,
	enItem_Gold_Pickaxe,
	enItem_Gold_Shovel,
	enItem_Gold_Axe,
	enItem_Gold_Hoe,

	//�h��B
	enItem_Gold_Helmet,
	enItem_Gold_ChestPlate,
	enItem_Gold_ChestPlate2,
	enItem_Gold_Leggings,
	enItem_Gold_Boots,

	//�_�C���̃c�[����
	enItem_Diamond_Sword,
	enItem_Diamond_Pickaxe,
	enItem_Diamond_Shovel,
	enItem_Diamond_Axe,
	enItem_Diamond_Hoe,

	//�h��B
	enItem_Diamond_Helmet,
	enItem_Diamond_ChestPlate,
	enItem_Diamond_ChestPlate2,
	enItem_Diamond_Leggings,
	enItem_Diamond_Boots,

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