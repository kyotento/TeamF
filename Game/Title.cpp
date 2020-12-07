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
	//todo ���ۂ̓X�^�[�g�{�^�����N���b�N���ꂽ�Ƃ��B
	if (GetKeyDown('G')) {

		NewGO<Game>();

		DeleteGO(this);
	}
}