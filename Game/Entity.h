//! @file
#pragma once

//! @brief �u���b�N�ł͂Ȃ��A������A�A�C�e���A��Ȃǂ̊��N���X�B
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity();

	virtual ~Entity();

	//! @brief ���W���擾�B
	virtual CVector3 GetPos() const = 0;

	//! @brief ���W��ݒ�B
	virtual void SetPos( const CVector3& pos ) = 0;
};

