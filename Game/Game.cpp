#include "stdafx.h"
#include "Game.h"
#include "Drop.h"
#include <random>
#include "Box.h"

Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_player);
}

bool Game::Start()
{
	m_player = NewGO<Player>();
	m_gameCamera = NewGO<GameCamera>();

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