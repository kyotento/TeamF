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
	//�K�v�ȃN���X�̐����B
	m_gameMode = NewGO<GameMode>();
	m_gameMode->SetName(L"gamemode");

	m_world = std::make_unique<World>();

	//���V�s�ǂݍ��݁B
	// TODO: World�̏��������O�ɓǂނƃG���[�ɂȂ�B�������̂킩��ɂ����ˑ��͂ǂ��ɂ��������B
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
	
	MouseCursor().SetLockMouseCursor(true);		//�}�E�X���Œ�B

	return true;
}

void Game::Update()
{
}

//�^�C�g���ւ̑J�ځB
void Game::TransToTitle()
{
	DeleteGO(this);
	NewGO<Title>();		
}