#pragma once
#include "World.h"
#include "GameMode.h"

class Player;
class GameCamera;
class Zombie;
class PlayerParameter;
class Title;
class ZombieGenerator;
class Game :public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();


private:
	World m_world;
	Player* m_player = nullptr;				//�v���C���[�B
	GameCamera* m_gameCamera = nullptr;		//�Q�[���J�����B
	Zombie* m_zombie = nullptr;				//�]���r�B
	GameMode* m_gameMode = nullptr;			//�Q�[�����[�h�B
	Title* m_title = nullptr;				//�^�C�g���B
	PlayerParameter* m_playerParameter = nullptr;		//�v���C���[�̃p�����[�^�[�B
	ZombieGenerator* m_zombieGenerator = nullptr;			//�]���r�c�N�[���B
};

