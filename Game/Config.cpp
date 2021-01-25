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
	//現在のチャンクを取得する。
	m_chunk = m_game->GetChunkRange();

	m_spriteRender[0].Init(L"Resource/spriteData/MinusButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/ChunkButton.dds");
	m_spriteRender[2].Init(L"Resource/spriteData/PlusButton.dds");
	m_spriteRender[3].Init(L"Resource/spriteData/DoneButton.dds");

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
	ClickProcess();
}


//スクリーン空間上の画像の範囲取得。
void Config::GetScPos()
{
	CVector2 imageSize[m_buttonNum];
	imageSize[0] = { 28.f,28.f };		//画像のサイズ。
	imageSize[1] = { 200.f,28.f };		//画像のサイズ。
	imageSize[2] = { 28.f,28.f };		//画像のサイズ。
	imageSize[3] = { 292.f,28.f };		//画像のサイズ。

	//todo Pivot対応させる。
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize[i].x / 1280.f * 0.5f) * m_scale,			//画像の左上。
							(m_position[i].y) + (imageSize[i].y / 720.f * 0.5f) * m_scale };
		m_scPos2[i] = { m_position[i].x + (imageSize[i].x / 1280.f * 0.5f) * m_scale,			//画像の右下。
							(m_position[i].y) - (imageSize[i].y / 720.f * 0.5f) * m_scale };
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
		if (Click() == 0) {			//-.
			m_chunk -= 1;
		}
		if (Click() == 2) {			//+.
			m_chunk += 1;
		}
		if (Click() == 3) {			//Done.
			if (m_backclass == enMenu) {
				m_game->NewEscMenu();
				DeleteGO(this);
				SetFogDistance(m_chunk * 1000);					//フォグが完全にかかりきる距離。
				SetFogStartDistance(m_chunk * 1000 - 200);		//フォグが始まる距離。
				m_game->SetChunkRange(m_chunk);
			}
			if (m_backclass == enTitle) {
				//タイトルのボタンを生成する。
				SetFogDistance(m_chunk * 1000);					//フォグが完全にかかりきる距離。
				SetFogStartDistance(m_chunk * 1000 - 200);		//フォグが始まる距離。
				m_game->SetChunkRange(m_chunk);
			}
		}
	}
}

void Config::PostRender() {

	wchar_t font[256];
	swprintf_s(font, L"%d", m_chunk);
	m_font.DrawScreenPos(font, { 631.f,320.f }, CVector4::White(), { 2.f,2.f },
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
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