#include "stdafx.h"
#include "PlayerParameter.h"
#include "Player.h"


PlayerParameter::PlayerParameter()
{
}


PlayerParameter::~PlayerParameter()
{
	DeleteGO(m_spriteRenderOnHand);
	DeleteGO(m_spriteRenderExp);
	DeleteGO(m_spriteRenderSelectItem);
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
	ChangeArmor();			//�h��͂�ύX����B
	SelectItem();			//�A�C�e����ύX����B
}

// �p�����[�^�摜�̊�Ղ𐶐��B
void PlayerParameter::SetParamFound()
{
	for (int i = 0; i < m_paramNum; i++) {

		//HP�̊�Չ摜�B
		m_hpPosition[i] = { 0.3f,0.855f };

		m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
		m_hpPosition[i].x += 0.02f * i;
		m_spriteRenderHP[i].SetPos(m_hpPosition[i]);
		m_spriteRenderHP[i].SetScale(m_scale);

		//�X�^�~�i�摜�̊�Ղ𐶐��B
		m_staminaPosition[i] = { 0.7f,0.855f };

		m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Max.dds");
		m_staminaPosition[i].x -= 0.02f * i;
		m_spriteRenderStamina[i].SetPos(m_staminaPosition[i]);
		m_spriteRenderStamina[i].SetScale(m_scale);

		//�h��͉摜�̊�ՁB
		m_armorPos[i] = { 0.3f,0.815f };

		m_spriteRenderArmor[i].Init(L"Resource/spriteData/Armor_Max.dds");
		m_armorPos[i].x += 0.02f * i;
		m_spriteRenderArmor[i].SetPos(m_armorPos[i]);
		m_spriteRenderArmor[i].SetScale(m_scale);
	}
	//�莝���A�C�e����ՁB
	m_spriteRenderOnHand = NewGO<CSpriteRender>();
	m_spriteRenderOnHand->Init(L"Resource/spriteData/OnHandInventory.dds");
	m_spriteRenderOnHand->SetPos({ 0.5f, 0.95f });
	m_spriteRenderOnHand->SetScale(1.7f);
	//�o���l��ՁB
	m_spriteRenderExp = NewGO<CSpriteRender>();
	m_spriteRenderExp->Init(L"Resource/spriteData/Experience_Found.dds");
	m_spriteRenderExp->SetPos({ 0.5f,0.89f });
	m_spriteRenderExp->SetScale(1.5f);
	//�A�C�e���Z���N�g�摜�B
	m_spriteRenderSelectItem = NewGO<CSpriteRender>();
	m_spriteRenderSelectItem->Init(L"Resource/spriteData/SelectInventory.dds");
	m_player->SetSelectItemNum(1);					//��Ԗڂ̃A�C�e����I�����Ă���B
	m_sItemPos = { 0.308f,0.95f };		//todo �����@��ړ����邲�Ƃ�0.048�B
	m_spriteRenderSelectItem->SetPos(m_sItemPos);
	m_spriteRenderSelectItem->SetScale(1.7f);
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

//�h��͂�ύX����B
void PlayerParameter::ChangeArmor()
{
	int ramainArmor = m_player->GetDefPow();		//�v���C���[�̖h��́B

	if (ramainArmor != m_oldArmor) {
		for (int i = 0; i < m_paramNum; i++) {
			if (i * 2 + 1 < ramainArmor) {
				m_spriteRenderArmor[i].Init(L"Resource/spriteData/Armor_Max.dds");
			}
			else{
				if (ramainArmor % 2 == 1 && i * 2 <= ramainArmor) {
					m_spriteRenderArmor[i].Init(L"Resource/spriteData/Armor_Half.dds");
				}
				else{
					m_spriteRenderArmor[i].Init(L"Resource/spriteData/Armor_Found.dds");
				}
			}
		}
	}
	m_oldArmor = ramainArmor;
}

//�A�C�e����I������B
void PlayerParameter::SelectItem()
{
	float moveX = 0.048f;		//�A�C�e������̈ړ���X�B

	m_selectNum -= GetMouseWheelNotch();		//�}�E�X�z�C�[���ɂ��w��B�B
	KariItemS();								//�L�[�{�[�h�ɂ��w��B

	if (m_selectNum != m_selectNumOld) {		//�l�ɕύX���s���Ă����ꍇ�B
		//���߂������̏C���B	
		if (m_selectNum > 9) {
			m_selectNum = 1;
		}
		if (m_selectNum < 1) {
			m_selectNum = 9;	
		}
		//���W�̕ύX�B
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem->SetPos(m_sItemPos);	
	}
	m_player->SetSelectItemNum(m_selectNum);	//�v���C���[�N���X�Ɋi�[�B	
	m_selectNumOld = m_selectNum;				//���݂̃A�C�e���ԍ����i�[�B
}

void PlayerParameter::KariItemS()
{
	if (GetKeyDown('1')) { m_selectNum = 1;}
	if (GetKeyDown('2')) { m_selectNum = 2;}
	if (GetKeyDown('3')) { m_selectNum = 3;}
	if (GetKeyDown('4')) { m_selectNum = 4;}
	if (GetKeyDown('5')) { m_selectNum = 5;}
	if (GetKeyDown('6')) { m_selectNum = 6;}
	if (GetKeyDown('7')) { m_selectNum = 7;}
	if (GetKeyDown('8')) { m_selectNum = 8;}
	if (GetKeyDown('9')) { m_selectNum = 9;}
}

void PlayerParameter::PostRender()
{
	//�o���l�B
	wchar_t font[256];
	swprintf_s(font, L"%d", m_player->GetExp());
	m_font.DrawScreenPos(font, { 631.f,610.f }, CVector4::Green(), { 0.5f,0.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}