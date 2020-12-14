#include "stdafx.h"
#include "Title.h"
#include "TitleSelect.h"
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

	//CVector2 vec(1, 2);
	//CVector2* vecp = new CVector2(1,2);

//	m_titleSelect = NewGO<TitleSelect>();

//	m_titleSelect.reset(new TitleSelect());			//���Ɠ��`�����A���̂ق��������i��O�͂������߂�ǂ��j�B
	m_titleSelect2 = std::make_unique<TitleSelect>();

	//std::unique_ptr<TitleSelect> tamada;


	//tamada = std::move(m_titleSelect);
	//tamada.reset();

	//tamada.release();
	//tamada.

	//TitleSelect* maaa = m_titleSelect.get();


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
	int clickNum = m_titleSelect2->Click();		//�N���b�N���ꂽ�{�^���̔ԍ��B

	//GameStart��I���������B
	if (clickNum == m_titleSelect2->en_buttonType_GameStart) {
		NewGO<Game>();
		DeleteGO(this);
	}
	//GameEnd�{�^����I���������B
	if (clickNum == m_titleSelect2->en_buttonType_GameEnd) {
		GetEngine().BreakGameLoop();
	}
	//Config��I���������B
	if (clickNum == m_titleSelect2->en_buttonType_Config) {
		MessageBox(nullptr, "�u���L�I�T�E���X�̂悤�Ɏ�𒷂����đ҂��ĂĂ�", "�܂���������ĂȂ���", MB_ICONASTERISK);
	}
}