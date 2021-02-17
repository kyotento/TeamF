#include "stdafx.h"
#include "Menu.h"
#include "Game.h"
#include "Config.h"
#include "Player.h"

Menu::Menu()
{
}


Menu::~Menu()
{
	MouseCursor().SetLockMouseCursor(true);		//マウスカーソルの非固定。
}

bool Menu::Start()
{
	m_spriteRender[0].Init(L"Resource/spriteData/ReturnToTitleButton.dds");
	m_spriteRender[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRender[2].Init(L"Resource/spriteData/ConfigurationButton.dds");
	m_spriteRender[3].Init(L"Resource/sprite/Button/BackToGame.png");

	m_clickName = L"Resource/soundData/game/click.wav";

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.3f };
		float posY = 0.125f * i;		//Y座標の加算値。
		m_position[i].y += posY;
		m_spriteRender[i].SetPos(m_position[i]);
		m_spriteRender[i].SetScale(m_scale);
	}
	MouseCursor().SetLockMouseCursor(false);		//マウスカーソルの非固定。
	return true;
}

void Menu::Update()
{
	GetScPos();				//スクリーン空間上の画像の範囲取得。
	ChangeColor();			//選択された画像の色を変更。
	ClickProcess();			// クリックしたときの処理。
}

//スクリーン空間上の画像の範囲取得。
void Menu::GetScPos()
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
void Menu::ChangeColor()
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

// クリックしたときの処理。
void Menu::ClickProcess()
{
	const int button = Click();

	//クリックされているなら音を鳴らす。
	if( button != m_buttonNum ){
		SuicideObj::CSE* se;
		se = NewGO<SuicideObj::CSE>( m_clickName );
		se->SetVolume( 0.1f );
		se->Play();
	}

	switch( button ){
	case enMenu_ReturnToTitle:
		//タイトルに戻る
		m_game->TransToTitle();
		break;

	case enMenu_GameEnd:
		//ゲームを終了する。
		DeleteGO( m_game );
		GetEngine().BreakGameLoop();
		break;

	case enMenu_Config:
		//コンフィグを開く。
		NewGO<Config>()->SetBackClass( Config::enMenu );
		m_game->DeleteEscMenu();
		break;

	case enMenu_BackToGame:
		//ゲームに戻る。
		m_game->SetIsEscMenu( false );
		m_game->DeleteEscMenu();
		break;
	}
}

//マウスをクリックしたときの判定。
int Menu::Click()
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