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
	m_world.Generate();

	m_player = NewGO<Player>();
	m_player->SetWorld( &m_world );
	m_gameCamera = NewGO<GameCamera>();
	m_zombie = NewGO<Zombie>();

	Box* box = NewGO<Box>();

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