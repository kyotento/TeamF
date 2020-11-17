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
	SetParamFound();		// パラメータ画像の基盤を生成。
	return true;
}

void PlayerParameter::Update()
{
	ChangeHP();				//体力を変更する。	
	ChangeStamina();		//スタミナを変更する。
	ChangeArmor();			//防御力を変更する。
}

// パラメータ画像の基盤を生成。
void PlayerParameter::SetParamFound()
{
	for (int i = 0; i < m_paramNum; i++) {

		//HPの基盤画像。
		m_hpPosition[i] = { 0.3f,0.855f };

		m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
		m_hpPosition[i].x += 0.02f * i;
		m_spriteRenderHP[i].SetPos(m_hpPosition[i]);
		m_spriteRenderHP[i].SetScale(m_scale);

		//スタミナ画像の基盤を生成。
		m_staminaPosition[i] = { 0.7f,0.855f };

		m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Max.dds");
		m_staminaPosition[i].x -= 0.02f * i;
		m_spriteRenderStamina[i].SetPos(m_staminaPosition[i]);
		m_spriteRenderStamina[i].SetScale(m_scale);

		//防御力画像の基盤。
		m_armorPos[i] = { 0.3f,0.815f };

		m_spriteRenderArmor[i].Init(L"Resource/spriteData/Armor_Max.dds");
		m_armorPos[i].x += 0.02f * i;
		m_spriteRenderArmor[i].SetPos(m_armorPos[i]);
		m_spriteRenderArmor[i].SetScale(m_scale);
	}
	//手持ちアイテム基盤。
	m_spriteRenderOnHand = NewGO<CSpriteRender>();
	m_spriteRenderOnHand->Init(L"Resource/spriteData/OnHandInventory.dds");
	m_spriteRenderOnHand->SetPos({ 0.5f, 0.95f });
	m_spriteRenderOnHand->SetScale(1.7f);
	//経験値基盤。
	m_spriteRenderExp = NewGO<CSpriteRender>();
	m_spriteRenderExp->Init(L"Resource/spriteData/Experience_Found.dds");
	m_spriteRenderExp->SetPos({ 0.5f,0.89f });
	m_spriteRenderExp->SetScale(1.5f);
}

//体力を変更する。
void PlayerParameter::ChangeHP()
{
	int remainHP = m_player->GetHP();		//プレイヤーの体力。

	if (remainHP != m_oldHP) {		//HPに変化が起きた時。

		for (int i = 0; i < m_paramNum; i++) {
			if (i * 2 + 1 < remainHP) {		
				m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
			}
			else{		
				if (remainHP % 2 == 1 && i * 2 <= remainHP) {		//残りHPが奇数且つ、影響を与える画像番号に到達した時。
					m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Half.dds");
				}
				else{			//偶数か、影響外のとき。
					m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Fond.dds");
				}
			}
		}
	}
	m_oldHP = remainHP;		//変更後のHPを格納。
}

//スタミナを変更する。
void PlayerParameter::ChangeStamina()
{
	int remainStamina = m_player->GetStamina();		//プレイヤーのスタミナ。

	if (remainStamina != m_oldStamina) {		//スタミナに変化が起きた時。

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

//防御力を変更する。
void PlayerParameter::ChangeArmor()
{
	int ramainArmor = m_player->GetDefPow();		//プレイヤーの防御力。

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

void PlayerParameter::PostRender()
{
	wchar_t font[256];
	swprintf_s(font, L"%d", m_player->GetExp());
	m_font.DrawScreenPos(font, { 631.f,610.f }, CVector4::Green(), { 0.5f,0.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}