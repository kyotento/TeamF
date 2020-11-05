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
}

// パラメータ画像の基盤を生成。
void PlayerParameter::SetParamFound()
{
	for (int i = 0; i < m_paramNum; i++) {

		//HPの基盤画像。
		m_hpPosition[i] = { 0.3f,0.8f };

		m_spriteRenderHP[i].Init(L"Resource/spriteData/HP_Max.dds");
		m_hpPosition[i].x += 0.02f * i;
		m_spriteRenderHP[i].SetPos(m_hpPosition[i]);
		m_spriteRenderHP[i].SetScale(m_scale);

		//スタミナ画像の基盤を生成。
		m_staminaPosition[i] = { 0.55f,0.8f };

		m_spriteRenderStamina[i].Init(L"Resource/spriteData/stamina_Max.dds");
		m_staminaPosition[i].x += 0.02f * i;
		m_spriteRenderStamina[i].SetPos(m_staminaPosition[i]);
		m_spriteRenderStamina[i].SetScale(m_scale);
	}
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