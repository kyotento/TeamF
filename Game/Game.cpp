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
#include "BossGenerator.h"
#include "Menu.h"
#include "Config.h"
#include "CowGenerator.h"

Game::Game()
{
}


Game::~Game()
{
	if (m_bgm) { DeleteGO(m_bgm); }
}

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

	m_bgmName = L"Resource/soundData/game/gamebgm.wav";
	m_clickName = L"Resource/soundData/game/click.wav";

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

	m_zombieGenerator.reset(NewGO<ZombieGenerator>());
	m_zombieGenerator->SetWorld(m_world.get());
	
	m_bossGenerator.reset(NewGO<BossGenerator>());
	m_bossGenerator->SetWorld(m_world.get());	
	
	m_cowGenerator.reset(NewGO<CowGenerator>());
	m_cowGenerator->SetWorld(m_world.get());

	MouseCursor().SetLockMouseCursor(true);		//マウスを固定。
	MouseCursor().SetShowMouseCursor( false );

	return true;
}

void Game::Update()
{
	m_world->SetChunkCoadRange(m_chunkRange);		//読み込みチャンクの更新。
	EscMenu();
	GameBGM();

	if (GetKeyDown(VK_F3)) {
		//デバッグ表示切り替え
		SetIsDebugDraw(!GetIsDebugDraw());
	}
}

void Game::EscMenu()
{
	SuicideObj::CSE* se;
	se = NewGO<SuicideObj::CSE>(m_clickName);
	se->SetVolume(0.1f);
	if (GetKeyDown(VK_ESCAPE)) {
		m_config = FindGO<Config>();
		if (m_menu == nullptr && !m_isEscMenu) {
			
			if ( m_config == nullptr) {
				NewEscMenu();
				se->Play();
			}
		}
		else{
			if (m_isEscMenu) {			//生成されているとき。
				se->Play();
				DeleteEscMenu();
				m_isEscMenu = false;
			}
		}
		if (m_config != nullptr)
		{
			DeleteGO(m_config);
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

void Game::GameBGM()
{
	if (m_stopCntBGM == 0) {
		//BGM
		if (!m_bgm) {
			m_bgm = NewGO<SuicideObj::CBGM>(m_bgmName);
			m_bgm->SetVolume(0.1f);
			m_bgm->Play(false, true);
		}
	}
	else {
		if (m_bgm) { DeleteGO(m_bgm); m_bgm = nullptr; }
	}
}