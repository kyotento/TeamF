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
{
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	//�K�v�ȃN���X�̐����B
	m_gameMode.reset(NewGO<GameMode>());
	m_gameMode->SetName(L"gamemode");

	m_world = std::make_unique<World>();

	m_bgmName = L"Resource/soundData/game/gamebgm.wav";
	m_clickName = L"Resource/soundData/game/click.wav";

	//���V�s�ǂݍ��݁B
	// TODO: World�̏��������O�ɓǂނƃG���[�ɂȂ�B�������̂킩��ɂ����ˑ��͂ǂ��ɂ��������B
	RecipeFiler recipeFiler;
	recipeFiler.SetFolder(L"Resource/recipeData/");
	recipeFiler.LoadRecipe(RecipeManager::Instance());

	//�v���C���[�̐����B
	Player* player = m_world->CreateEntity<Player>();
	m_world->SetPlayer( player );
	player->SetName(L"player");
	player->SetGameIns(this);

	m_gameCamera = std::make_unique<GameCamera>();

	m_sun.reset(NewGO<Sun>());

	m_zombieGenerator.reset(NewGO<ZombieGenerator>());
	m_zombieGenerator->SetWorld(m_world.get());
	//
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
}

void Game::EscMenu()
{
	SuicideObj::CSE* se;
	se = NewGO<SuicideObj::CSE>(m_clickName);
	se->SetVolume(0.1f);
	if (GetKeyDown(VK_ESCAPE)) {
		if (m_menu == nullptr && !m_isEscMenu) {
			m_config = FindGO<Config>();
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
	if (!m_isBgmFlag) {
		//BGM
		m_bgm = NewGO<SuicideObj::CSE>(m_bgmName);
		m_bgm->SetVolume(0.1f);
		m_bgm->Play();
		m_isBgmFlag = true;
	}
	if (!m_bgm->GetIsPlaying())
	{
		m_isBgmFlag = false;
	}
}