#include "stdafx.h"
#include "Config.h"
#include "Game.h"

Config::Config()
{
}


Config::~Config()
{
}

bool Config::Start()
{
	m_game = FindGO<Game>();

	m_spriteRender[0].Init(L"Resource/spriteData/ReturnToTitleButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRender[2].Init(L"Resource/spriteData/GameEndButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.4f };
		float posY = 0.125f * i;		//Y座標の加算値。
		m_position[i].y += posY;
		m_spriteRender[i].SetPos(m_position[i]);
		m_spriteRender[i].SetScale(m_scale);
	}

	MouseCursor().SetLockMouseCursor(false);		//マウスカーソルの非固定。

	return true;
}

void Config::Update()
{
	GetScPos();
	ChangeColor();
}


//スクリーン空間上の画像の範囲取得。
void Config::GetScPos()
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
void Config::ChangeColor()
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

//画像をクリックしたときの処理。
void Config::ClickProcess()
{
	if (GetKeyDown(VK_LBUTTON)) {		//左クリックした時。
		if (Click() == 0) {

		}
		if (Click() == 2) {
			if (m_backclass == enMenu) {
				m_game->NewEscMenu();
			}
			if (m_backclass == enTitle) {
				//タイトルのボタンを生成する。
			}
		}

	}
}

//マウスをクリックしたときの判定。
int Config::Click()
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