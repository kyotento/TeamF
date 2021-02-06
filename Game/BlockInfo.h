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

	//enCube_None�p��Info
	static const BlockInfo NONE_BLOCK;
};