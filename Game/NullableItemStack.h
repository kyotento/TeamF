//! @file
#pragma once

#include "ItemType.h"

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

	//! @brief �X�^�b�N������擾�B
	virtual int GetStackLimit() const{
		return 0;
	}

	//! @brief �A�C�e��id���擾�B
	virtual int GetID() const{
		return enCube_None;
	}

	//! @brief �A�C�e���̑������擾�B
	virtual int GetToolID() const {
		return enTool_None;
	}

	//! @brief �H����
	virtual bool IsFood() const{
		return false;
	}

	//! @brief �R����
	virtual bool IsFuel() const{
		return false;
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