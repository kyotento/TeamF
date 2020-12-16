#include "stdafx.h"
#include "PlayerDeath.h"


PlayerDeath::PlayerDeath()
{
}

PlayerDeath::~PlayerDeath()
{
}

bool PlayerDeath::Start()
{
	m_spriteRender[0].Init(L"Resource/spriteData/RespawnButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/ReturnToTitleButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.6f };
		float posY = 0.125f * i;		//Y座標の加算値。
		m_position[i].y += posY;
		m_spriteRender[i].SetPos(m_position[i]);
		m_spriteRender[i].SetScale(m_scale);
	}
	MouseCursor().SetLockMouseCursor(false);		//マウスカーソルの非固定。
	return true;
}

void PlayerDeath::Update()
{
	GetScPos();				//スクリーン空間上の画像の範囲取得。
	ChangeColor();			//選択された画像の色を変更。
}

//スクリーン空間上の画像の範囲取得。
void PlayerDeath::GetScPos()
{
	CVector2 imageSize = { 292.f,28.f };		//画像のサイズ。

	//todo Pivot対応させる。
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize.x / 1280.f * 0.5f) * m_scale,			//画像の左上。
							(m_position[i].y) + (imageSize.y / 720.f * 0.5f) * m_scale };
		m_scPos2[i] = { m_position[i].x + (imageSize.x / 1280.f * 0.5f) * m_scale,			//画像の右下。
							(m_position[i].y) - (imageSize.y / 720.f * 0.5f) * m_scale };
	}
}

//選択された画像の色を変更。
void PlayerDeath::ChangeColor()
{
	CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//マウスカーソルの座標。

	for (int i = 0; i < m_buttonNum; i++) {
		//画像内にマウスカーソルがあるとき。
		if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y
			&& mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
			m_spriteRender[i].SetColor(CVector4::Green());
		}
		else {		//ないとき。
			m_spriteRender[i].SetColor(CVector4::White());
		}
	}
}

//マウスをクリックしたときの判定。
int PlayerDeath::Click()
{
	if (GetKeyDown(VK_LBUTTON)) {		//右クリックした時。

		CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//マウスカーソルの座標。

		for (int i = 0; i < m_buttonNum; i++) {
			//画像内にマウスカーソルがあるとき。
			if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y
				&& mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
				return i;
			}
		}
	}

	return m_buttonNum;		//画像の番号外の数値。
}