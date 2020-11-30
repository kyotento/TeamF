#include "stdafx.h"
#include "TitleSelect.h"
#include "GUIManager.h"


TitleSelect::TitleSelect()
{
	//m_spriteRneder[0].Init(L"Resource/spriteData/GameStartButton.dds");
	//m_spriteRneder[1].Init(L"Resource/spriteData/GameEndButton.dds");
	//m_spriteRneder[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	//for (int i = 0; i < m_buttonNum; i++) {
	//	m_position[i] = { 0.5f,0.5f };
	//	float posY = 0.1f * i;
	//	m_position[i].y += posY;
	//	m_spriteRneder[i].SetPos(m_position[i]);
	//	m_spriteRneder[i].SetScale(m_scale);
	//}

//	GUI::GUIManager::Instance().AddRoot(this);		//最上位のNodeを登録。
}

TitleSelect::~TitleSelect()
{
}

bool TitleSelect::Start()
{
	m_spriteRneder[0].Init(L"Resource/spriteData/GameStartButton.dds");
	m_spriteRneder[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRneder[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.5f };
		float posY = 0.1f * i;
		m_position[i].y += posY;
		m_spriteRneder[i].SetPos(m_position[i]);
		m_spriteRneder[i].SetScale(m_scale);
	}

	return true;
}

void TitleSelect::Update()
{
	GetScPos();				//スクリーン空間上の画像の範囲取得。
	ChangeColor();			//選択された画像の色を変更。
}

//スクリーン空間上の画像の範囲取得。
void TitleSelect::GetScPos()
{
	CVector2 imageSize = { 292.f,28.f };		//画像のサイズ。

	//todo Pivot対応させる。
	for (int i = 0; i < m_buttonNum; i++) {
		m_scPos[i] = { (m_position[i].x) - (imageSize.x / 1280.f * 0.5f) * m_scale,			//画像の左上。
							(m_position[i].y) + (imageSize.y / 720.f * 0.5f) * m_scale};
		m_scPos2[i] = { m_position[i].x + (imageSize.x / 1280.f * 0.5f) * m_scale,			//画像の右下。
							(m_position[i].y) - (imageSize.y / 720.f * 0.5f) * m_scale};
	}
}

//選択された画像の色を変更。
void TitleSelect::ChangeColor()
{
	CVector3 mousePos = MouseCursor().GetMouseCursorPos();			//マウスカーソルの座標。

	for (int i = 0; i < m_buttonNum; i++) {
		//画像内にマウスカーソルがあるとき。
		if (mousePos.x >= m_scPos[i].x && mousePos.y <= m_scPos[i].y
			&& mousePos.x <= m_scPos2[i].x && mousePos.y >= m_scPos2[i].y) {
			m_spriteRneder[i].SetColor(CVector4::Green());
		}
		else {		//ないとき。
			m_spriteRneder[i].SetColor(CVector4::White());
		}
	}
}

//マウスをクリックしたときの判定。
int TitleSelect::Click()
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


//CVector2 TitleSelect::GetSize() const 
//{
//	CVector2 scale = { 292.f,28.f };
//	scale *= m_scale;
//	return scale;
//}