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