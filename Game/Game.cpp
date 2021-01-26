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
#include "Menu.h"
#include "Config.h"
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

	m_world = std::make_unique<World>();

	//レシピ読み込み。
	// TODO: Worldの初期化より前に読むとエラーになる。いつかこのわかりにくい依存はどうにかしたい。
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder(L"Resource/recipeData/");
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	//プレイヤーの生成。
	Player* player = m_world->CreateEntity<Player>();
	m_world->SetPlayer( player );
	player->SetName(L"player");
	player->SetGameIns(this);

	m_gameCamera = std::make_unique<GameCamera>();

	m_sun.reset(NewGO<Sun>());

	//m_zombieGenerator.reset(NewGO<ZombieGenerator>());
	//m_zombieGenerator->SetWorld(m_world.get());
	
	m_cowGenerator.reset(NewGO<CowGenerator>());
	m_cowGenerator->SetWorld(m_world.get());

	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。

	return true;
}

void Game::Update()
{
	m_world->SetChunkCoadRange(m_chunkRange);		//読み込みチャンクの更新。
	EscMenu();
}

void Game::EscMenu()
{
	if (GetKeyDown(VK_ESCAPE)) {
		if (m_menu == nullptr && !m_isEscMenu) {
			m_config = FindGO<Config>();
			if ( m_config == nullptr) {
				NewEscMenu();
			}
		}
		else{
			if (m_isEscMenu) {			//生成されているとき。
				DeleteEscMenu();
				m_isEscMenu = false;
			}
		}
	}
}

//EscMnuを生成する。
void Game::NewEscMenu()
{
	m_menu.reset(NewGO<Menu>());
	m_menu->SetGame(this);
	m_isEscMenu = true;			//生成フラグを返す。
}

//EscMnuを消す処理。
void Game::DeleteEscMenu()
{
	m_menu.reset();
}

//タイトルへの遷移。
void Game::TransToTitle()
{
	DeleteGO(this);
	NewGO<Title>();		
}