#include "stdafx.h"
#include "Box.h"
#include "Drop.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Zombie.h"

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
	m_zombie = NewGO<Zombie>();
	m_zombie->SetPos( CVector3( 20, 15, 15 ) * Block::WIDTH );
	m_world.AddEntity( m_zombie );

	Box* box = NewGO<Box>();

	//todo 何これ？？by佐伯。
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

}