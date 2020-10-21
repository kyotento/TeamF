#include "stdafx.h"
#include "GameMode.h"

GameMode::GameMode()
{
	m_gameMode = enGameModeSurvival;		//default�̃Q�[�����[�h���T�o�C�o���ɁB
}


GameMode::~GameMode()
{
}

bool GameMode::Start()
{

	return true;
}

void GameMode::Update()
{
	if (GetKeyInput(VK_CONTROL)) {			//�R���g���[���������Ă���Ƃ��B
		if (GetKeyDown('S')) {
			m_gameMode = enGameModeSurvival;		//�T�o�C�o���ɁB
		}
		if (GetKeyDown('C')) {
			m_gameMode = enGameModeCreative;		//�N���G�C�e�B�u�ɁB
		}
	}
}

void GameMode::PostRender()
{
	wchar_t font[256];
	if (m_gameMode == enGameModeSurvival) {
		swprintf_s(font, L"GameMode::Survival\n");
	}
	else {
		swprintf_s(font, L"GameMode::Creative\n");
	}
	m_font.DrawScreenPos(font, { 1000.f,0.f }, CVector4::White(), { 0.6f,0.6f }, 
		CVector2::Zero(),
		0.0f,
		DirectX::SpriteEffects_None,
		0.7f
	);
}