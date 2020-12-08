#include "stdafx.h"
#include "Inventory.h"
#include "DropItem.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Zombie.h"
#include "PlayerParameter.h"
#include "BlockType.h"
#include "BlockFactory.h"

Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_player);
}

bool Game::Start()
{
	//必要なクラスの生成。
	m_gameMode = NewGO<GameMode>();
	m_gameMode->SetName(L"gamemode");

	m_player = NewGO<Player>(&m_world);
	m_player->SetName(L"player");
	m_gameCamera = NewGO<GameCamera>();

	m_playerParameter = NewGO<PlayerParameter>();
	m_playerParameter->SetPlayerIns(m_player);

	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。
	//プレイヤーのインベントリを確認するためのアイテム生成だったり。

	/*for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			std::random_device rand;
			Drop* drop = NewGO<Drop>();
			drop->SetEnCube(EnCube(rand() % enCube_Num));
			drop->SetPosition(CVector3(i, 15, j));
			drop->SetNumber((rand() % 10) + 1);
		}
	}*/

	return true;
}

void Game::Update()
{
}