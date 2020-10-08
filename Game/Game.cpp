#include "stdafx.h"
#include "Game.h"

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

	return true;
}

void Game::Update()
{

}