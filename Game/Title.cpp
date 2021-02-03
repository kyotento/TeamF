#include "stdafx.h"
#include "Title.h"
#include "TitleSelect.h"
#include "Config.h"
#include "Game.h"

Title::Title()
{
}

Title::~Title()
{
	DeleteGO(m_spriteRender);
}

bool Title::Start()
{
	m_spriteRender = NewGO<GameObj::CSpriteRender>();
	m_spriteRender->Init(L"Resource/spriteData/Title.dds");
	m_spriteRender->SetPos(CVector3::Zero());
	m_spriteRender->SetPivot(CVector2::Zero());

//	m_titleSelect.reset(new TitleSelect());			//下と同義だが、下のほうがいい（例外はいた時めんどい）。
	NewTitleSelect();			//TitleSelectを生成。

	MouseCursor().SetLockMouseCursor(false);		//マウスを非固定。

	return true;
}

void Title::Update()
{
	ClickProcess();		//画像をクリックしたときの処理。
}

//画像をクリックしたときの処理。
void Title::ClickProcess()
{
	int clickNum = m_titleSelect->Click();		//クリックされたボタンの番号。

	//GameStartを選択した時。
	if (clickNum == m_titleSelect->en_buttonType_GameStart) {
		NewGO<Game>();
		DeleteGO(this);
	}
	//GameEndボタンを選択した時。
	if (clickNum == m_titleSelect->en_buttonType_GameEnd) {
		GetEngine().BreakGameLoop();
	}
	//Configを選択した時。
	//if (clickNum == m_titleSelect->en_buttonType_Config) {
	//	Config* config = NewGO<Config>();
	//	config->SetTitle(this);
	//	config->SetBackClass(config->enTitle);
	//}
}

//TitleSelectを生成。
void Title::NewTitleSelect()
{
	m_titleSelect = std::make_unique<TitleSelect>();
}

//TitleSelectを消す。
void Title::DeleteTitleSelect()
{
	m_titleSelect.reset();
}
