#include "stdafx.h"
#include "PlayerParameter.h"
#include "Player.h"
#include "ItemStack.h"
#include "ItemDisplay.h"

namespace{
	//! アイテムバーの左端のアイテムの座標。
	const CVector2 leftItemPos = { 0.308f,0.95f };
	//! アイテムバーで、右隣りのアイテムとの幅。一つ右に移動する場合のX座標の移動量。
	const float itemBarStride = 0.048f;
	//アイテム欄一つの移動量X。
	float moveX = itemBarStride;
	//前のID
	int OldID = 0;
	//インベントリでクリックしたときにアイテム切り替えのフラグ。
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
	SetParamFound();		// パラメータ画像の基盤を生成。

	//右手表示のclassにゅうごー
	InitRightHand();
	return true;
}

void PlayerParameter::Update()
{
	ChangeHP();				//体力を変更する。
	if (!m_player->GetIsPlayerDead()) {	
		ChangeStamina();		//スタミナを変更する。
		ChangeArmor();			//防御力を変更する。
		ChangeExp();			//経験値ゲージを変更する。
		SelectItem();			//アイテムを変更する。
		PlayerLevelUp();
	}
	//こいつ更新させてあげてください。
	m_rightHandDisplay->SetPos(m_player->GetPos());

	//テキストメッセージ
	m_textDrawTimer = max(0.0f,m_textDrawTimer - GetDeltaTimeSec());
	//睡眠
	if (m_player->GetIsSleep()) {
		m_textDrawTimer = 2.0f;
	}
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
	m_spriteRenderOnHand.Init(L"Resource/spriteData/OnHandInventory.dds");
	m_spriteRenderOnHand.SetPos({ 0.5f, 0.95f });
	m_spriteRenderOnHand.SetScale(1.7f);
	m_spriteRenderOnHand.SetLayerDepth( 0.6f );
	//経験値基盤。
	m_spriteRenderExp.Init(L"Resource/spriteData/Experience_Found.dds");
	m_spriteRenderExp.SetPos({ 0.5f,0.89f });
	m_spriteRenderExp.SetScale(m_expScale);
	//経験値ゲージ。
	m_spriteRenderExpGauge.Init(L"Resource/spriteData/Experience_Gauge.dds");
	m_spriteRenderExpGauge.SetPos({ 0.288f,0.89f });
	m_spriteRenderExpGauge.SetScale(m_expScale);
	m_spriteRenderExpGauge.SetPivot({ 0.f, 0.5f });
	m_spriteRenderExpGauge.SetColor({ 0.000000000f, 0.501960814f, 0.000000000f, 0.20000000f });		//緑色の半透明画像に。
	//アイテムセレクト画像。
	m_spriteRenderSelectItem.Init(L"Resource/spriteData/SelectInventory.dds");
	m_player->SetSelectItemNum(1);					//一番目のアイテムを選択している。
	m_sItemPos = leftItemPos;		//todo メモ　一つ移動するごとに0.048。
	m_spriteRenderSelectItem.SetPos(m_sItemPos);
	m_spriteRenderSelectItem.SetScale(1.7f);
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

//経験値ゲージを変更する。
void PlayerParameter::ChangeExp()
{
	float expGaugeScaleX = m_player->GetExp() - (int)m_player->GetExp();
	m_spriteRenderExpGauge.SetScale({ expGaugeScaleX * m_expScale , m_expScale });
}

//アイテムを選択する。
void PlayerParameter::SelectItem()
{

	m_selectNum -= GetMouseWheelNotch();		//マウスホイールによる指定。
	KariItemS();								//キーボードによる指定。
	StopMoveToChange();

	if (m_selectNum == m_selectNumOld && !m_isStopMoveToChangeFlag) {
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem.SetPos(m_sItemPos);
		m_isItemChangeFlag = false;
	}
	else if (m_selectNum != m_selectNumOld || m_isStopMoveToChangeFlag) {	//値に変更が行われていた場合。
		//超過した時の修正。	
		if (m_selectNum > 9) {
			m_selectNum = 1;
		}
		if (m_selectNum < 1) {
			m_selectNum = 9;
		}
		//座標の変更。
		m_sItemPos.x = m_selectPosX + m_selectNum * moveX;
		m_spriteRenderSelectItem.SetPos(m_sItemPos);
		m_isItemChangeFlag = true;
		m_isStopMoveToChangeFlag = false;

	}

	m_player->SetSelectItemNum(m_selectNum);					//プレイヤークラスに格納。
	m_rightHandDisplay->SetChangeItemFlag(m_isItemChangeFlag);	//右手にフラグも格納します。
	m_selectNumOld = m_selectNum;								//現在のアイテム番号を格納。

	if (!m_rightHandDisplay->GetEndChangeFlag())
	{
		m_isItemChangeFlag = false;
		m_isStopMoveToChangeFlag = false;
	}
}

//1~9ボタンによるアイテムセレクト(ごり押しの極み)。
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
	//経験値。
	wchar_t font[256];
	swprintf_s(font, L"%d", (int)m_player->GetExp());
	m_font.DrawScreenPos(font, { 631.f,610.f }, CVector4::Green(), { 0.5f,0.5f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);

	//手持ちアイテムの描画。アイテムのDraw関数はScreenPos基準だから、変換する。
	const float frameW = GetGraphicsEngine().GetFrameBuffer_W();
	const float frameH = GetGraphicsEngine().GetFrameBuffer_H();

	CVector2 itemPos = leftItemPos;
	itemPos.x *= frameW;
	itemPos.y *= frameH;

	const float moveX = itemBarStride * frameW;

	Inventory& inv = m_player->GetInventory();

	const float scale = 1.5f;

	//インベントリの0～8のスロットにあるアイテムを描画する。
	for( int i = 0; i <= 8; i++ ){
		inv.GetNullableItem( i ).Draw( itemPos, scale );
		itemPos.x += moveX;
	}

	//睡眠状態
	if (m_textDrawTimer > 0.0f) {
		m_font.Draw(L"リスポーン地点が変更されました。", { 0.0f , 0.7f }, CVector4::White(), 0.5f, { 0.0f, 0.0f });
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