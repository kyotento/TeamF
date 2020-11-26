//! @file
#pragma once
#include "ItemStack.h"

//! @brief �N���t�g���V�s��\������N���X�B
//! @details ���V�s�́A���A�����A�A�C�e���z��A���ʕ������B
//! @details �Ⴆ�Ώ����Ȃ�A��1�A����2�A�A�C�e���z��[�ΒY,�_]�A�ł���B
//! @author Takayama
class Recipe{
public:

	Recipe(int width, int height, const std::vector<int>& itemArray, const ItemStack& result);
	~Recipe();

	bool operator==( const Recipe& rhs );

	bool operator!=( const Recipe& rhs ){
		return !(operator==( rhs ));
	}

	//! @brief ���A�����A���ʕ��͖������āA�A�C�e���z�񂾂����r���ē������ǂ�����Ԃ��B
	bool LightEquals( const Recipe& rhs );

	//! @brief ���V�s�̕��ƍ������擾�B
	//! @details std::pair �̑�1�v�f�����A��2�v�f�������B
	std::pair<int, int> GetWH(){
		return m_widthHeight;
	}

	//! @brief ���V�s�̐��ʕ���Ԃ��B
	std::unique_ptr<ItemStack> GetResult(){
		return std::make_unique<ItemStack>( m_result );
	}

private:
	std::pair<int, int> m_widthHeight;
	std::vector<int> m_itemArray;
	ItemStack m_result;
};

