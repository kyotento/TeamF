#include "stdafx.h"
#include "GameMode.h"

GameMode::GameMode()
{
	m_gameMode = enGameModeSurvival;		//defaultのゲームモードをサバイバルに。
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
	if (GetKeyInput(VK_CONTROL)) {			//コントロールを押しているとき。
		if (GetKeyDown('S')) {
			m_gameMode = enGameModeSurvival;		//サバイバルに。
		}
		if (GetKeyDown('C')) {
			m_gameMode = enGameModeCreative;		//クリエイティブに。
		}
	}
}