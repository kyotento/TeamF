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

//	m_titleSelect.reset(new TitleSelect());			//���Ɠ��`�����A���̂ق��������i��O�͂������߂�ǂ��j�B
	NewTitleSelect();			//TitleSelect�𐶐��B

	MouseCursor().SetLockMouseCursor(false);		//�}�E�X���Œ�B

	return true;
}

void Title::Update()
{
	ClickProcess();		//�摜���N���b�N�����Ƃ��̏����B
}

//�摜���N���b�N�����Ƃ��̏����B
void Title::ClickProcess()
{
	int clickNum = m_titleSelect->Click();		//�N���b�N���ꂽ�{�^���̔ԍ��B

	//GameStart��I���������B
	if (clickNum == m_titleSelect->en_buttonType_GameStart) {
		NewGO<Game>();
		DeleteGO(this);
	}
	//GameEnd�{�^����I���������B
	if (clickNum == m_titleSelect->en_buttonType_GameEnd) {
		GetEngine().BreakGameLoop();
	}
	//Config��I���������B
	//if (clickNum == m_titleSelect->en_buttonType_Config) {
	//	Config* config = NewGO<Config>();
	//	config->SetTitle(this);
	//	config->SetBackClass(config->enTitle);
	//}
}

//TitleSelect�𐶐��B
void Title::NewTitleSelect()
{
	m_titleSelect = std::make_unique<TitleSelect>();
}

//TitleSelect�������B
void Title::DeleteTitleSelect()
{
	m_titleSelect.reset();
}
