//! @file
#pragma once
#include "../BlockType.h"

class Player;

//! @brief �u���b�N�̏��ƃ��f�������_�[�����N���X�B
//! @details �����ɂ� BlockFactory ���g�p���Ăق����B
//! @details ����̔h���N���X��������� BlockFactory::CreateBlock �֐��ɒǉ�����B
//! @author Takayama
class Block{
public:
	Block();
	virtual ~Block();

	//! @brief �|�W�V�������Z�b�g�B
	//! @details World��1�u���b�N1�P�ʂō��W���^�p���Ă��邽�߁A���f���ɂ̓u���b�N�̕�����Z�����l��ݒ肵�Ă���B
	void SetPos( int x, int y, int z );

	GameObj::CInstancingModelRender& GetModel(){
		return m_model;
	}

	//! @brief �u���b�N�̎�ނ�ݒ�B
	//! @details ���̊֐��̓��f����ύX���Ȃ��B
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	//! @brief �u���b�N�̎�ނ��擾�B
	EnCube GetBlockType() const
	{
		return m_state;
	}

	//! @brief ���f���̕`������邩�ǂ�����ݒ�B
	//! @details ���܂��Ă��Č����Ȃ��u���b�N�̕`����~�߂邽�߂Ɏg�p�����B
	void SetIsDraw(bool isDraw){
		m_model.SetIsDraw( isDraw );
	}

	bool IsCollisionEnabled() const{
		return m_collision.operator bool();
	}

	//! @brief �u���b�N���E�N���b�N�����Ƃ��̓���B
	//! @return true ���������s�����B(�u���b�N�̐ݒu�����Ȃ�)
	//! @return false �������s���Ȃ������B(�u���b�N�̐ݒu������)
	virtual bool OnClick( Player* player ){
		return false;
	}

	void EnableCollision();

	void DisableCollision(){
		m_collision.reset();
	}

	//! @brief �u���b�N�̕��A���s���A�����B
	static constexpr float WIDTH = 140;
private:
	GameObj::CInstancingModelRender m_model;
	//! @brief �u���b�N�̎�ށB
	EnCube m_state = enCube_None;

	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;
};

