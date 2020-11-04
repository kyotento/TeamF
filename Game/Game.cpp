#include "stdafx.h"
#include "Box.h"
#include "Drop.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Zombie.h"
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

	m_player = NewGO<Player>();
	m_player->SetName(L"player");
	m_player->SetWorld( &m_world );
	m_gameCamera = NewGO<GameCamera>();

	Box* box = NewGO<Box>();

	//プレイヤーのインベントリを確認するためのアイテム生成だったり。
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			std::random_device rand;
			Drop* drop = NewGO<Drop>();
			drop->SetEnCube(EnCube(rand() % enCube_Num));
			drop->SetPosition(CVector3(i, 15, j));
			drop->SetNumber((rand() % 10) + 1);
		}
	}

	return true;
}

void Game::Update()
{
	//以下ブロックの設置と破壊のお試し。
	if (GetKeyDown('R')) {
		m_world.DeleteBlock((m_player->GetPos() / Block::WIDTH) + m_player->GetFront() * 2);
	}
	else if (GetKeyDown('F')) {
		CVector3 pos = m_player->GetPos() / Block::WIDTH + m_player->GetFront() * 2;
		/*for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				for (int o = 0; o < 5; o++) {
					m_world.PlaceBlock(CVector3(pos.x + i, pos.y + j, pos.z + o), BlockFactory::CreateBlock(enCube_Grass));
				}
			}
		}*/
		m_world.PlaceBlock(pos,BlockFactory::CreateBlock(enCube_Grass));
	}
}