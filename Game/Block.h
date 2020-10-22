//! @file
#pragma once
#include "../BlockType.h"

//! @brief �u���b�N�̏��ƃ��f�������_�[�����N���X�B
//! @details �����ɂ� BlockFactory ���g�p���Ăق����B
//! @author Takayama
class Block{
public:
	Block();
	~Block();

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
	//! @brief �u���b�N�̎�ނ�ݒ�B
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

