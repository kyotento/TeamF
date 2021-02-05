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
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	m_spriteRender = NewGO<GameObj::CSpriteRender>();
	m_spriteRender->Init(L"Resource/spriteData/Title.dds");
	m_spriteRender->SetPos(CVector3::Zero());
	m_spriteRender->SetPivot(CVector2::Zero());

	m_bgmName = L"Resource/soundData/game/titlebgm.wav";
	m_clickName = L"Resource/soundData/game/click.wav";
//	m_titleSelect.reset(new TitleSelect());			//���Ɠ��`�����A���̂ق��������i��O�͂������߂�ǂ��j�B
	NewTitleSelect();			//TitleSelect�𐶐��B

	MouseCursor().SetLockMouseCursor(false);		//�}�E�X���Œ�B

	return true;
}

void Title::Update()
{
	ClickProcess();		//�摜���N���b�N�����Ƃ��̏����B
	TitleBGM();
}

//�摜���N���b�N�����Ƃ��̏����B
void Title::ClickProcess()
{
	int clickNum = m_titleSelect->Click();		//�N���b�N���ꂽ�{�^���̔ԍ��B
	SuicideObj::CSE* se;
	se = NewGO<SuicideObj::CSE>(m_clickName);
	se->SetVolume(0.1f);
	//GameStart��I���������B
	if (clickNum == m_titleSelect->en_buttonType_GameStart) {
		se->Play();
		NewGO<Game>();
		DeleteGO(this);
	}
	//GameEnd�{�^����I���������B
	if (clickNum == m_titleSelect->en_buttonType_GameEnd) {
		se->Play();
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

void Title::TitleBGM()
{
	if (!m_isBgmFlag) {
		//BGM
		m_bgm = NewGO<SuicideObj::CSE>(m_bgmName);
		m_bgm->SetVolume(0.1f);
		m_bgm->Play();
		m_isBgmFlag = true;
	}
	if (!m_bgm->GetIsPlaying())
	{
		m_isBgmFlag = false;
	}
}