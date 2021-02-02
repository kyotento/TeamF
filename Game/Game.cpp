#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "ItemDictionary.h"
#include "BlockType.h"
#include "BlockFactory.h"
#include "RecipeFiler.h"
#include "RecipeManager.h"
#include "Sun.h"
#include "Title.h"
#include "ZombieGenerator.h"
#include "Menu.h"
#include "CowGenerator.h"

Game::Game()
{
}


Game::~Game()
{}

bool Game::Start()
{
	//必要なクラスの生成。
	m_gameMode.reset(NewGO<GameMode>());
	m_gameMode->SetName(L"gamemode");

	//ブロックファクトリ初期化。
	BlockFactory::Init( L"Resource/jsonData/blockData/" );

	//アイテムを読み込む。
	ItemDictionary::Instance().LoadItems( L"Resource/jsonData/itemData/" );
	ItemDictionary::Instance().LoadBlocks( BlockFactory::GetBlockMap() );

	//レシピ読み込み。
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder(L"Resource/jsonData/recipeData/");
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	m_world = std::make_unique<World>();

	//プレイヤーの生成。
	Player* player = m_world->CreateEntity<Player>();
	m_world->SetPlayer( player );
	player->SetName(L"player");
	player->SetGameIns(this);

	m_gameCamera = std::make_unique<GameCamera>();

	m_sun.reset(NewGO<Sun>());

	m_zombieGenerator.reset(NewGO<ZombieGenerator>());
	m_zombieGenerator->SetWorld(m_world.get());
	
	m_cowGenerator.reset(NewGO<CowGenerator>());
	m_cowGenerator->SetWorld(m_world.get());

	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。

	return true;
}

void Game::Update()
{
	EscMenu();
}

void Game::EscMenu()
{
	if (GetKeyDown(VK_ESCAPE)) {
		if (m_menu == nullptr) {
			m_menu.reset(NewGO<Menu>());
			m_menu->SetGame(this);
		}
		else {
			m_menu.reset();
		}
	}
}

//タイトルへの遷移。
void Game::TransToTitle()
{
	DeleteGO(this);
	NewGO<Title>();		
}