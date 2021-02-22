#pragma once
#include "ItemType.h"
#include "BlockType.h"
#include "AABB.h"

//! @brief �u���b�N�̎�ނ��Ƃɋ��ʂ̏��B
class BlockInfo{
public:
	//ID
	EnCube id = EnCube::enCube_None;
	//���O
	std::string name;
	//�u���b�N�̑ϋv�l
	int hp = 1;
	//�j��ɗL���ȃc�[��
	EnTool usefulTool = enTool_None;
	//���f���p�X
	std::filesystem::path modelPath;
	//AABB
	std::vector<AABB> aabbArray;
	//���邳
	char light = 0;
	//�s�����Ȃ�true
	bool isOpacity = true;
	//�e�N�X�`���ɓ����v�f���邩?
	bool isTransTexture = false;
	//�j�󎞂ɗ��Ƃ��A�C�e��
	EnItem dropItem = EnItem::enAllItem_Num;
	//�A�C�R���p�X�v���C�g�p�X
	std::filesystem::path spritePath;
	//�h���b�v���f���p�X�@��Ɏ��Ƃ��̃��f���ł�����
	std::filesystem::path dropModelPath;

	//�R�����x��
	float fuel_level = 0;

	//�L�����R���ƏՓ˂��邩
	bool isColision = true;

	//enCube_None�p��Info
	static const BlockInfo NONE_BLOCK;
};