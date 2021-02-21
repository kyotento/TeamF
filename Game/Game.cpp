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
	//�K�v�ȃN���X�̐����B
	m_gameMode.reset(NewGO<GameMode>());
	m_gameMode->SetName(L"gamemode");

	//�u���b�N�t�@�N�g���������B
	BlockFactory::Init( L"Resource/jsonData/blockData/" );

	//�A�C�e����ǂݍ��ށB
	ItemDictionary::Instance().LoadItems( L"Resource/jsonData/itemData/" );
	ItemDictionary::Instance().LoadBlocks( BlockFactory::GetBlockMap() );

	m_bgmName = L"Resource/soundData/game/gamebgm.wav";
	m_clickName = L"Resource/soundData/game/click.wav";

	//���V�s�ǂݍ��݁B
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder(L"Resource/jsonData/recipeData/");
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	m_world = std::make_unique<World>();

	//�v���C���[�̐����B
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

	MouseCursor().SetLockMouseCursor(true);		//�}�E�X���Œ�B

	return true;
}

void Game::Update()
{
	m_world->SetChunkCoadRange(m_chunkRange);		//�ǂݍ��݃`�����N�̍X�V�B
	EscMenu();
	GameBGM();

	if (GetKeyDown(VK_F3)) {
		//�f�o�b�O�\���؂�ւ�
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
			if (m_isEscMenu) {			//��������Ă���Ƃ��B
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

//EscMnu�𐶐�����B
void Game::NewEscMenu()
{
	m_menu.reset(NewGO<Menu>());
	m_menu->SetGame(this);
	m_isEscMenu = true;			//�����t���O��Ԃ��B
}

//EscMnu�����������B
void Game::DeleteEscMenu()
{
	m_menu.reset();
}

//�^�C�g���ւ̑J�ځB
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