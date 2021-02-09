//! @file
#pragma once
#include "NullableItemStack.h"
#include "Item.h"

//! @brief �A�C�e���Ƃ��̐������N���X�B�R�s�[�\�B
//! @author Takayama
class ItemStack : public NullableItemStack{
public:

	//! @brief �A�C�e���X�^�b�N����num��ʂ̃X�^�b�N�Ƃ��Ď��o���B
	static std::unique_ptr<ItemStack> TakeItem( std::unique_ptr<ItemStack>& item, int num );

	ItemStack(Item& item , int number = 1) : m_item(item), m_number(number){}

	bool operator==( const ItemStack& rhs ){
		return m_number == rhs.m_number && &m_item == &( rhs.m_item );
	}

	bool operator!=( const ItemStack& rhs ){
		return !( operator==( rhs ) );
	}

	//! @brief ����ݒ�B
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief �����擾�B
	int GetNumber() const override{
		return m_number;
	}

	//! @brief �A�C�e�������擾�B
	const Item& GetItem() const{
		return m_item;
	}

	//! @brief �A�C�e�������擾�B
	Item& GetItem(){
		return m_item;
	}

	//! @brief �A�C�e����3D���f���̃p�X���擾�B
	std::filesystem::path GetModelPath() const{
		return m_item.GetModelPath();
	}

	//! @brief �A�C�e��id���擾�B
	unsigned GetID() const override{
		return m_item.GetID();
	}

	//! @brief �A�C�e���̑������擾�B
	unsigned GetToolID() const override {
		return m_item.GetToolID();
	}

	//! @brief ���̃A�C�e���̃c�[�����x��(��:1�`�_�C��:5)���擾
	int GetToolLevel() const {
		return m_item.GetToolLevel();
	}

	//! @brief �X�^�b�N������擾�B
	int GetStackLimit() const{
		return m_item.GetStackLimit();
	}

	/// <summary>
	/// �u���b�N���A�c�[�������擾����B
	/// </summary>
	/// <returns>true�Ȃ�u���b�N</returns>
	bool GetIsBlock() override{
		return m_item.GetIsBlock();
	}

	//! @brief �`��֐�
	void Draw( const CVector2& pos, const CVector2& parentScale ) override;

private:
	CFont m_font;
	Item& m_item;
	int m_number = 0;
};

