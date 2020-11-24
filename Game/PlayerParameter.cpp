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
	SetParamFound();		// パラメータ画像の基盤を生成。
	return true;
}

void PlayerParameter::Update()
{
	ChangeHP();				//体力を変更する。	
	ChangeStamina();		//スタミナを変更する。
	ChangeArmor();			//防御力を変更する。
	SelectItem();			//アイテムを変更する。
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
	//アイテムセレクト画像。
	m_spriteRenderSelectItem = NewGO<CSpriteRender>();
	m_spriteRenderSelectItem->Init(L"Resource/spriteData/SelectInventory.dds");
	m_player->SetSelectItemNum(1);					//一番目のアイテムを選択している。
	m_sItemPos = { 0.308f,0.95f };		//todo メモ　一つ移動するごとに0.048。
	m_spriteRenderSelectItem->SetPos(m_sItemPos);
	m_spriteRenderSelectItem->SetScale(1.7f);
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

//アイテムを選択する。
void PlayerParameter::SelectItem()
{
	float moveX = 0.048f;		//アイテム欄一つの移動量X。

	m_selectNum -= GetMouseWheelNotch();		//マウスホイールによる指定。。
	KariItemS();								//キーボードによる指定。

	if (m_selectNum != m_selectNumOld) {		//値に変更が行われていた場合。
		//超過した時の修正。	
		if (m_selectNum > 9) {
			m_selectNum = 1;
		}
		if (m_selectNum < 1) {
			m_selectNum = 9;	
		}
		//座標の変更。
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem->SetPos(m_sItemPos);	
	}
	m_player->SetSelectItemNum(m_selectNum);	//プレイヤークラスに格納。	
	m_selectNumOld = m_selectNum;				//現在のアイテム番号を格納。
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
	//経験値。
	wchar_t font[256];
	swprintf_s(font, L"%d", m_player->GetExp());
	m_font.DrawScreenPos(font, { 631.f,610.f }, CVector4::Green(), { 0.5f,0.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}