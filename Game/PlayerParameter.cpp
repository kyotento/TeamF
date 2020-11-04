#include "stdafx.h"
#include "PlayerParameter.h"
#include "Player.h"


PlayerParameter::PlayerParameter()
{
}


PlayerParameter::~PlayerParameter()
{
}

bool PlayerParameter::Start()
{
	SetParamFound();		// �p�����[�^�摜�̊�Ղ𐶐��B
	return true;
}

void PlayerParameter::Update()
{
	ChangeHP();				//�̗͂�ύX����B	
	ChangeStamina();		//�X�^�~�i��ύX����B
}

// �p�����[�^�摜�̊�Ղ𐶐��B
void PlayerParameter::SetParamFound()
{
	for (int i = 0; i < m_paramNum; i++) {

		//HP�̊�Չ摜�B
		m_hpPosition[i] = { 0.3f,0.8f };

		m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
		m_hpPosition[i].x += 0.02f * i;
		m_spriteRenderHP[i].SetPos(m_hpPosition[i]);
		m_spriteRenderHP[i].SetScale(m_scale);

		//�X�^�~�i�摜�̊�Ղ𐶐��B
		m_staminaPosition[i] = { 0.55f,0.8f };

		m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Max.dds");
		m_staminaPosition[i].x += 0.02f * i;
		m_spriteRenderStamina[i].SetPos(m_staminaPosition[i]);
		m_spriteRenderStamina[i].SetScale(m_scale);
	}
}

//�̗͂�ύX����B
void PlayerParameter::ChangeHP()
{
	int remainHP = m_player->GetHP();		//�v���C���[�̗̑́B

	if (remainHP != m_oldHP) {		//HP�ɕω����N�������B

		for (int i = 0; i < m_paramNum; i++) {
			if (i * 2 + 1 < remainHP) {		
				m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
			}
			else{		
				if (remainHP % 2 == 1 && i * 2 <= remainHP) {		//�c��HP������A�e����^����摜�ԍ��ɓ��B�������B
					m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Half.dds");
				}
				else{			//�������A�e���O�̂Ƃ��B
					m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Fond.dds");
				}
			}
		}
	}
	m_oldHP = remainHP;		//�ύX���HP���i�[�B
}

//�X�^�~�i��ύX����B
void PlayerParameter::ChangeStamina()
{
	int remainStamina = m_player->GetStamina();		//�v���C���[�̃X�^�~�i�B

	if (remainStamina != m_oldStamina) {		//�X�^�~�i�ɕω����N�������B

		for (int i = 0; i < m_paramNum; i++) {
			if (i * 2 + 1 < remainStamina) {
				m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Max.dds");
			}
			else {
				if (remainStamina % 2 == 1 && i * 2 <= remainStamina) {
					m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Half.dds");
				}
				else {
					m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Found.dds");
				}
			}
		}
	}
	m_oldStamina = remainStamina;
}