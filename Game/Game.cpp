#include "stdafx.h"
#include "Inventory.h"
#include "DropItem.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Zombie.h"
#include "BlockType.h"
#include "BlockFactory.h"
#include "RecipeFiler.h"
#include "RecipeManager.h"
#include "Sun.h"

Game::Game()
{
}


Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_sun);
}

bool Game::Start()
{
	//レシピ読み込み
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder( L"Resource/recipeData/" );
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	//必要なクラスの生成。
	m_gameMode = NewGO<GameMode>();
	m_gameMode->SetName(L"gamemode");

	m_player = NewGO<Player>(&m_world);
	m_player->SetName(L"player");

	m_gameCamera = NewGO<GameCamera>();

	m_sun = NewGO<Sun>();

	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。

	return true;
}

void Game::Update()
{
}

void Game::GameEnd()
{

}