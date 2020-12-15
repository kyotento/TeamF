//! @file
#pragma once

#include "BlockType.h"

//! @brief ItemStack�̊��N���X�B�p������Ă��Ȃ����̃N���X�P�̂ŁAnull�I�u�W�F�N�g��\���B
//! @author Takayama
class NullableItemStack{
public:

	static NullableItemStack& Instance(){
		static NullableItemStack nullObj;
		return nullObj;
	}

	//! @brief �����擾�B
	virtual int GetNumber() const{
		return 0;
	}

	//! @brief �A�C�e��id���擾�B
	virtual unsigned GetID() const{
		return enCube_None;
	}

	/// <summary>
	/// �u���b�N���A�c�[�������擾����B
	/// </summary>
	/// <returns>true�Ȃ�u���b�N</returns>
	virtual bool GetIsBlock(){
		return false;
	}

	//! @brief �`��֐�
	virtual void Draw( const CVector2& pos, const CVector2& parentScale ){}

protected:
	NullableItemStack(){}
};