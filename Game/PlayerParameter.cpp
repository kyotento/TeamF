#include "stdafx.h"
#include "PlayerParameter.h"
#include "Player.h"
#include "ItemStack.h"
#include "ItemDisplay.h"

namespace{
	//! �A�C�e���o�[�̍��[�̃A�C�e���̍��W�B
	const CVector2 leftItemPos = { 0.308f,0.95f };
	//! �A�C�e���o�[�ŁA�E�ׂ�̃A�C�e���Ƃ̕��B��E�Ɉړ�����ꍇ��X���W�̈ړ��ʁB
	const float itemBarStride = 0.048f;
	//�A�C�e������̈ړ���X�B
	float moveX = itemBarStride;
	//�O��ID
	int OldID = 0;
	//�C���x���g���ŃN���b�N�����Ƃ��ɃA�C�e���؂�ւ��̃t���O�B
	bool isSwichByMouseClickFlag = false;

	int oldexp = 0;
}

PlayerParameter::PlayerParameter()
{
}


PlayerParameter::~PlayerParameter()
{ 
	DeleteGO(m_rightHandDisplay); 
}

bool PlayerParameter::Start()
{
	SetParamFound();		// �p�����[�^�摜�̊�Ղ𐶐��B

	//�E��\����class�ɂイ���[
	InitRightHand();
	return true;
}

void PlayerParameter::Update()
{
	ChangeHP();				//�̗͂�ύX����B
	if (!m_player->GetIsPlayerDead()) {	
		ChangeStamina();		//�X�^�~�i��ύX����B
		ChangeArmor();			//�h��͂�ύX����B
		ChangeExp();			//�o���l�Q�[�W��ύX����B
		SelectItem();			//�A�C�e����ύX����B
		PlayerLevelUp();
	}
	//�����X�V�����Ă����Ă��������B
	m_rightHandDisplay->SetPos(m_player->GetPos());

	//�e�L�X�g���b�Z�[�W
	m_textDrawTimer = max(0.0f,m_textDrawTimer - GetDeltaTimeSec());
	//����
	if (m_player->GetIsSleep()) {
		m_textDrawTimer = 2.0f;
	}
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
	m_spriteRenderOnHand.Init(L"Resource/spriteData/OnHandInventory.dds");
	m_spriteRenderOnHand.SetPos({ 0.5f, 0.95f });
	m_spriteRenderOnHand.SetScale(1.7f);
	m_spriteRenderOnHand.SetLayerDepth( 0.6f );
	//�o���l��ՁB
	m_spriteRenderExp.Init(L"Resource/spriteData/Experience_Found.dds");
	m_spriteRenderExp.SetPos({ 0.5f,0.89f });
	m_spriteRenderExp.SetScale(m_expScale);
	//�o���l�Q�[�W�B
	m_spriteRenderExpGauge.Init(L"Resource/spriteData/Experience_Gauge.dds");
	m_spriteRenderExpGauge.SetPos({ 0.288f,0.89f });
	m_spriteRenderExpGauge.SetScale(m_expScale);
	m_spriteRenderExpGauge.SetPivot({ 0.f, 0.5f });
	m_spriteRenderExpGauge.SetColor({ 0.000000000f, 0.501960814f, 0.000000000f, 0.20000000f });		//�ΐF�̔������摜�ɁB
	//�A�C�e���Z���N�g�摜�B
	m_spriteRenderSelectItem.Init(L"Resource/spriteData/SelectInventory.dds");
	m_player->SetSelectItemNum(1);					//��Ԗڂ̃A�C�e����I�����Ă���B
	m_sItemPos = leftItemPos;		//todo �����@��ړ����邲�Ƃ�0.048�B
	m_spriteRenderSelectItem.SetPos(m_sItemPos);
	m_spriteRenderSelectItem.SetScale(1.7f);
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

//�o���l�Q�[�W��ύX����B
void PlayerParameter::ChangeExp()
{
	float expGaugeScaleX = m_player->GetExp() - (int)m_player->GetExp();
	m_spriteRenderExpGauge.SetScale({ expGaugeScaleX * m_expScale , m_expScale });
}

//�A�C�e����I������B
void PlayerParameter::SelectItem()
{

	m_selectNum -= GetMouseWheelNotch();		//�}�E�X�z�C�[���ɂ��w��B
	KariItemS();								//�L�[�{�[�h�ɂ��w��B
	StopMoveToChange();

	if (m_selectNum == m_selectNumOld && !m_isStopMoveToChangeFlag) {
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem.SetPos(m_sItemPos);
		m_isItemChangeFlag = false;
	}
	else if (m_selectNum != m_selectNumOld || m_isStopMoveToChangeFlag) {	//�l�ɕύX���s���Ă����ꍇ�B
		//���߂������̏C���B	
		if (m_selectNum > 9) {
			m_selectNum = 1;
		}
		if (m_selectNum < 1) {
			m_selectNum = 9;
		}
		//���W�̕ύX�B
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem.SetPos(m_sItemPos);
		m_isItemChangeFlag = true;
		m_isStopMoveToChangeFlag = false;

	}

	m_player->SetSelectItemNum(m_selectNum);					//�v���C���[�N���X�Ɋi�[�B
	m_rightHandDisplay->SetChangeItemFlag(m_isItemChangeFlag);	//�E��Ƀt���O���i�[���܂��B
	m_selectNumOld = m_selectNum;								//���݂̃A�C�e���ԍ����i�[�B

	if (!m_rightHandDisplay->GetEndChangeFlag())
	{
		m_isItemChangeFlag = false;
		m_isStopMoveToChangeFlag = false;
	}
}

//1~9�{�^���ɂ��A�C�e���Z���N�g(���艟���̋ɂ�)�B
void PlayerParameter::KariItemS()
{
	for (int i = 1; i <= 9; i++) {
		if (GetKeyDown('0' + i)) { m_selectNum = i; }
	}
}

void PlayerParameter::InitRightHand()
{
	m_rightHandDisplay = NewGO<ItemDisplay>();
	m_rightHandDisplay->SetName(L"ItemDisplay");
	m_rightHandDisplay->SetPos(m_player->GetPos());
}

void PlayerParameter::PostRender()
{
	//�o���l�B
	wchar_t font[256];
	swprintf_s(font, L"%d", (int)m_player->GetExp());
	m_font.DrawScreenPos(font, { 631.f,610.f }, CVector4::Green(), { 0.5f,0.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);

	//�莝���A�C�e���̕`��B�A�C�e����Draw�֐���ScreenPos�������A�ϊ�����B
	const float frameW = GetGraphicsEngine().GetFrameBuffer_W();
	const float frameH = GetGraphicsEngine().GetFrameBuffer_H();

	CVector2 itemPos = leftItemPos;
	itemPos.x *= frameW;
	itemPos.y *= frameH;

	const float moveX = itemBarStride * frameW;

	Inventory& inv = m_player->GetInventory();

	const float scale = 1.5f;

	//�C���x���g����0�`8�̃X���b�g�ɂ���A�C�e����`�悷��B
	for( int i = 0; i <= 8; i++ ){
		inv.GetNullableItem( i ).Draw( itemPos, scale );
		itemPos.x += moveX;
	}

	//�������
	if (m_textDrawTimer > 0.0f) {
		m_font.Draw(L"���X�|�[���n�_���ύX����܂����B", { 0.0f , 0.7f }, CVector4::White(), 0.5f, { 0.0f, 0.0f });
	}
}

void PlayerParameter::StopMoveToChange()
{
	auto& item = m_player->GetInventory().GetItem(m_player->GetSelectItemNum()-1);
	if (item != nullptr)
	{
		if (item->GetID() != OldID)
		{
			m_isStopMoveToChangeFlag = true;
			isSwichByMouseClickFlag = false;
		}
		else if (isSwichByMouseClickFlag)
		{
			m_isStopMoveToChangeFlag = true;
			isSwichByMouseClickFlag = false;
		}
		else if (item->GetID() == OldID)
		{
			m_isStopMoveToChangeFlag = false;
			isSwichByMouseClickFlag = false;
		}
		OldID = item->GetID();
	}
	else if (item == nullptr && !isSwichByMouseClickFlag)
	{
		m_isStopMoveToChangeFlag = true;
		isSwichByMouseClickFlag = true;
	}
}

void PlayerParameter::PlayerLevelUp()
{
	if (oldexp != (int)m_player->GetExp())
	{
		SuicideObj::CSE* upse;
		upse = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/levelup.wav");
		upse->SetVolume(1.0f);
		upse->Play();
	}
	oldexp = m_player->GetExp();
}