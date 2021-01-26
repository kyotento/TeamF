//! @file
#pragma once
#include "../BlockType.h"
#include "Light.h"

class Player;
class World;

//! @brief �u���b�N�̏��ƃ��f�������_�[����N���X�B
//! @details �����ɂ� BlockFactory ��g�p���Ăق����B
//! @details ����̔h���N���X�������� BlockFactory::CreateBlock �֐��ɒǉ�����B
//! @author Takayama
class Block{
public:
	Block();
	virtual ~Block();

	//! @brief �|�W�V������Z�b�g�B
	//! @details World��1�u���b�N1�P�ʂō��W��^�p���Ă��邽�߁A���f���ɂ̓u���b�N�̕����Z�����l��ݒ肵�Ă���B
	void SetPos( int x, int y, int z );
		
	//! @brief ���[���h���W��g���ă|�W�V������Z�b�g�B
	//! ��������Ȃ��Ȃ�֐�����
	void SetPosWithWorldPos(const CVector3& worldpos);

	//! @brief ���f���̃|�W�V������擾
	const CVector3& GetModelPos()const {
		return m_model.GetPos();
	}

	//! @brief ���f���������
	void InitModel(const wchar_t* filePath);

	//! @brief �u���b�N�̎�ނ�ݒ�B
	//! @details ���̊֐��̓��f����ύX���Ȃ��B
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	//! @brief �u���b�N�̎�ނ�擾�B
	EnCube GetBlockType() const
	{
		return m_state;
	}

	//! @brief ���f���̕`�����邩�ǂ�����ݒ�B
	//! @details ���܂��Ă��Č����Ȃ��u���b�N�̕`���~�߂邽�߂Ɏg�p�����B
	void SetIsDraw(bool isDraw){
		m_model.SetIsDraw( isDraw );
	}

	//! @brief �����蔻�肪�L�����擾�B
	bool IsCollisionEnabled() const{
		return m_collision.operator bool();
	}

	//! @brief �u���b�N��E�N���b�N�����Ƃ��̓���B
	//! @return true ��������s�����B(�u���b�N�̐ݒu����Ȃ�)
	//! @return false ������s���Ȃ������B(�u���b�N�̐ݒu�����)
	virtual bool OnClick( Player* player ){
		return false;
	}

	//! @brief �����蔻��̗L�����B
	void EnableCollision();

	//! @brief �����蔻��̖������B
	void DisableCollision(){
		m_collision.reset();
	}
	void SetHP(const int hp)
	{
		m_maxHP = hp;
		m_hp = hp;
	}
	//HP��擾
	const int GetHP() const
	{
		return m_hp;
	}
	//HP����炷
	void ReduceHP(const int attack)
	{
		m_hp -= attack;
	}
	//�u���b�N�̑ϋv�l��S��������
	void RestoresBlockDurabilityValue()
	{
		m_hp = m_maxHP;
	}

	//! @brief ���C�e�B���O��Ԃ̐ݒ�B
	void SetLightingData(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "���邳���x�����͈͊O�ł�")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		if (column < 2) {
			m_lighting.m[row][column] = LightUtil::DRAWING_LIGHT[lightpower];//�u���b�N���C�g
		}
		else {
			m_lighting.m[row][column] = LightUtil::DRAWING_SKY_LIGHT[lightpower];//�X�J�C���C�g
		}
	}
	//! @brief ���C�e�B���O��s���B
	void Lighting(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "���邳���x�����͈͊O�ł�")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		//��薾�邢����g�p
		if (column < 2) {
			m_lighting.m[row][column] = max(m_lighting.m[row][column], LightUtil::DRAWING_LIGHT[lightpower]);//�u���b�N���C�g
		}
		else {
			m_lighting.m[row][column] = max(m_lighting.m[row][column], LightUtil::DRAWING_SKY_LIGHT[lightpower]);//�X�J�C���C�g
		}
	}
	//! @brief ���C�e�B���O�`��̍X�V�B
	void RefleshDrawLighting(World* world, const IntVector3& blockPos, char lightPower, char skyLightPower);

	//! @brief �u���b�N�̕��A���s���A�����B
	static constexpr float WIDTH = 140;

private:
	//! @brief ���C�e�B���O�v�Z����
	void CalcAddLight(bool isDestroy = false);

private:
	//���f��
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;//���C�g���p

	//! @brief �u���b�N�̎�ށB
	EnCube m_state = enCube_None;

	//���邳
	CMatrix m_lighting = CMatrix::Zero();
	//int m_blockLighting[6] = {};
	//int m_skyLighting[6] = {};
	//���̃u���b�N���e���͈͓�ɂ�������̃��X�g
	//std::list<Block*> m_lightSourceList;
	//���̃u���b�N�������Ƃ��ĉe����^���Ă���u���b�N�̃��X�g
	//std::list<Block*> m_lightingBlockList;

	//�R���W����
	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;
	int m_maxHP = 0;
	int m_hp = 10;
};

