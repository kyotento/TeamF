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
#include "CowGenerator.h"

Game::Game()
{
}


Game::~Game()
{}

bool Game::Start()
{
	//�K�v�ȃN���X�̐����B
	m_gameMode.reset(NewGO<GameMode>());
	m_gameMode->SetName(L"gamemode");

	m_world = std::make_unique<World>();

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

	//m_zombieGenerator.reset(NewGO<ZombieGenerator>());
	//m_zombieGenerator->SetWorld(m_world.get());
	
	m_cowGenerator.reset(NewGO<CowGenerator>());
	m_cowGenerator->SetWorld(m_world.get());

	MouseCursor().SetLockMouseCursor(true);		//�}�E�X���Œ�B

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

//�^�C�g���ւ̑J�ځB
void Game::TransToTitle()
{
	DeleteGO(this);
	NewGO<Title>();		
}