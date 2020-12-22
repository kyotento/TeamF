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
#include "Title.h"
#include "ZombieGenerator.h"

Game::Game()
{
}


Game::~Game()
{
//	DeleteGO(m_player);
	DeleteGO(m_sun);
}

bool Game::Start()
{
	//必要なクラスの生成。
	m_gameMode = NewGO<GameMode>();
	m_gameMode->SetName(L"gamemode");

	m_world = std::make_unique<World>();

	//レシピ読み込み。
	// TODO: Worldの初期化より前に読むとエラーになる。いつかこのわかりにくい依存はどうにかしたい。
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder(L"Resource/recipeData/");
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	m_player.reset(NewGO<Player>(m_world.get()));
	m_player->SetName(L"player");
	m_player->SetGameIns(this);

	m_gameCamera = std::make_unique<GameCamera>();

	m_sun = NewGO<Sun>();

	//m_zombieGenerator = NewGO<ZombieGenerator>();
	//m_zombieGenerator->SetWorld(m_world.get());
	
	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。

	return true;
}

void Game::Update()
{
}

//タイトルへの遷移。
void Game::TransToTitle()
{
	DeleteGO(this);
	NewGO<Title>();		
}